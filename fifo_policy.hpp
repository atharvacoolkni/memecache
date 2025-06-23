#ifndef FIFO_CACHE_POLICY_HPP
#define FIFO_CACHE_POLICY_HPP

#include "cache_policy.hpp"
#include <list>
#include <unordered_map>
#include <stdexcept>

namespace caches
{
    /**
     * FIFO (First-In, First-Out) Cache Policy
     * Oldest inserted element is removed first when cache is full.
     */
    template <typename Key>
    class FIFOCachePolicy : public ICachePolicy<Key>
    {
    public:
        using fifo_iterator = typename std::list<Key>::const_iterator;

        FIFOCachePolicy() = default;
        ~FIFOCachePolicy() noexcept override = default;

        void Insert(const Key& key) override {
            if (key_lookup.find(key) != key_lookup.end()) return; // avoid duplicates

            fifo_queue.emplace_front(key);
            key_lookup[key] = fifo_queue.begin();
        }

        void Touch(const Key& key) noexcept override {
            // FIFO does not care about accesses
            (void)key;
        }

        void Erase(const Key& key) noexcept override {
            auto it = key_lookup.find(key);
            if (it == key_lookup.end()) return; // key not found

            fifo_queue.erase(it->second);
            key_lookup.erase(it);
        }

        const Key& ReplacementCandidate() const override {
            if (fifo_queue.empty()) {
                throw std::runtime_error("No keys available for eviction (FIFO).");
            }
            return fifo_queue.back(); // oldest inserted
        }

    private:
        std::list<Key> fifo_queue;
        std::unordered_map<Key, fifo_iterator> key_lookup;
    };
}

#endif // FIFO_CACHE_POLICY_HPP
