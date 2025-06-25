#ifndef LIFO_CACHE_POLICY_HPP
#define LIFO_CACHE_POLICY_HPP

#include "cache_policy.hpp"
#include <list>
#include <unordered_map>
#include <stdexcept>

namespace caches
{
    /*
     * LIFO (Last-In, First-Out) Cache Policy
     * Evicts the most recently inserted key when full.
     */
    template <typename Key>
    class LIFOCachePolicy : public ICachePolicy<Key>
    {
    public:
        using lifo_iterator = typename std::list<Key>::const_iterator;

        LIFOCachePolicy() = default;
        ~LIFOCachePolicy() noexcept override = default;

        void Insert(const Key& key) override {
            if (key_lookup.find(key) != key_lookup.end()) return; // Avoid duplicates

            lifo_stack.emplace_front(key);
            key_lookup[key] = lifo_stack.begin();
        }

        void Touch(const Key& key) noexcept override {
            // LIFO does not care about accesses
            (void)key;
        }

        void Erase(const Key& key) noexcept override {
            auto it = key_lookup.find(key);
            if (it == key_lookup.end()) return;

            lifo_stack.erase(it->second);
            key_lookup.erase(it);
        }

        const Key& ReplacementCandidate() const override {
            if (lifo_stack.empty()) {
                throw std::runtime_error("No keys available for eviction (LIFO).");
            }
            return lifo_stack.front(); // most recently inserted
        }

    private:
        std::list<Key> lifo_stack;
        std::unordered_map<Key, lifo_iterator> key_lookup;
    };
}

#endif 