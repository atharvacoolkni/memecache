#ifndef CACHE_HPP
#define CACHE_HPP

#include "cache_policy.hpp"
#include <cstddef>
#include <functional>
#include <stdexcept>
#include <unordered_map>

namespace caches
{
    /*
     * Fixed-size cache using a customizable eviction policy.
     * Key - Type of the key (must be hashable)
     * Value - Type of value
     * Policy - Eviction policy class template (like LRUCachePolicy, FIFOCachePolicy, etc.)
     * HashMap - Map container (default: std::unordered_map)
     */
    template <typename Key, typename Value, template <typename> class Policy = NoCachePolicy,
            typename HashMap = std::unordered_map<Key, Value>>
    class fixed_sized_cache
    {
    public:
        using iterator = typename HashMap::iterator;
        using const_iterator = typename HashMap::const_iterator;
        using on_erase_cb = typename std::function<void(const Key&, const Value&)>;

        /*
         * Constructor
         * max_size - Maximum number of elements in the cache
         * policy - Eviction policy to be used
         * on_erase - Optional callback when an item is evicted
         */
        explicit fixed_sized_cache(
            size_t max_size,
            const Policy<Key>& policy = Policy<Key>{},
            on_erase_cb on_erase = [](const Key&, const Value&) {})
            : cache_policy{policy},
              max_cache_size{max_size},
              on_erase_callback{on_erase}
        {
            if (max_cache_size == 0)
            {
                throw std::invalid_argument{"Cache size must be greater than zero."};
            }
        }

        ~fixed_sized_cache() noexcept { Clear(); }

        // Adds or updates an entry
        void Put(const Key& key, const Value& value) noexcept
        {
            auto element = cache_items_map.find(key);

            if (element == cache_items_map.end())
            {
                if (cache_items_map.size() >= max_cache_size)
                {
                    const Key& evict_key = cache_policy.ReplacementCandidate();
                    Erase(evict_key);
                }
                Insert(key, value);
            }
            else
            {
                Update(key, value);
            }
        }

        // Try to get element by key; returns pair<iterator, found>
        std::pair<const_iterator, bool> TryGet(const Key& key) noexcept
        {
            auto element = cache_items_map.find(key);

            if (element != cache_items_map.end())
            {
                cache_policy.Touch(key);
                return {element, true};
            }

            return {element, false};
        }

        // Get value by key, throws if key not found
        const Value& Get(const Key& key)
        {
            auto result = TryGet(key);
            if (!result.second)
            {
                throw std::range_error("Key not found in cache.");
            }
            return result.first->second;
        }

        // Check if a key exists
        bool Cached(const Key& key) const noexcept
        {
            return cache_items_map.find(key) != cache_items_map.end();
        }

        // Return number of entries
        std::size_t Size() const noexcept { return cache_items_map.size(); }

        // Remove a key, return true if it existed
        bool Remove(const Key& key)
        {
            auto it = cache_items_map.find(key);
            if (it == cache_items_map.end()) return false;

            Erase(it);
            return true;
        }

        // Remove everything
        void Clear()
        {
            for (const auto& [key, value] : cache_items_map)
            {
                cache_policy.Erase(key);
            }
            cache_items_map.clear();
        }

        const_iterator begin() const noexcept { return cache_items_map.cbegin(); }
        const_iterator end() const noexcept { return cache_items_map.cend(); }

    private:
        void Insert(const Key& key, const Value& value)
        {
            cache_policy.Insert(key);
            cache_items_map.emplace(key, value);
        }

        void Update(const Key& key, const Value& value)
        {
            cache_policy.Touch(key);
            cache_items_map[key] = value;
        }

        void Erase(const_iterator it)
        {
            cache_policy.Erase(it->first);
            on_erase_callback(it->first, it->second);
            cache_items_map.erase(it);
        }

        void Erase(const Key& key)
        {
            auto it = cache_items_map.find(key);
            if (it != cache_items_map.end())
            {
                Erase(it);
            }
        }

        HashMap cache_items_map;
        Policy<Key> cache_policy;
        std::size_t max_cache_size;
        on_erase_cb on_erase_callback;
    };
} // namespace caches

#endif // CACHE_HPP
