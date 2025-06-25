#ifndef LRU_CACHE_POLICY_HPP
#define LRU_CACHE_POLICY_HPP

#include "cache_policy.hpp"
#include <list>
#include <unordered_map>
#include <stdexcept>

namespace caches
{
    /*
     * LRU (Least Recently Used) Cache Policy
     * Evicts the least recently accessed key.
     */
    template <typename Key>
    class LRUCachePolicy : public ICachePolicy<Key>
    {
    public:
        using lru_iterator = typename std::list<Key>::iterator;

        LRUCachePolicy() = default;
        ~LRUCachePolicy() noexcept override = default;

        void Insert(const Key& key) override {
            if (key_finder.find(key) != key_finder.end()) return; // prevent duplicate

            lru_queue.emplace_front(key);
            key_finder[key] = lru_queue.begin();
        }

        void Touch(const Key& key) override {
            auto it = key_finder.find(key);
            if (it == key_finder.end()) return;

            lru_queue.splice(lru_queue.begin(), lru_queue, it->second); // move to front
        }

        void Erase(const Key& key) noexcept override {
            auto it = key_finder.find(key);
            if (it == key_finder.end()) return;

            lru_queue.erase(it->second);
            key_finder.erase(it);
        }

        const Key& ReplacementCandidate() const override {
            if (lru_queue.empty()) {
                throw std::runtime_error("No keys available for eviction (LRU).");
            }
            return lru_queue.back(); // least recently used
        }

    private:
        std::list<Key> lru_queue;
        std::unordered_map<Key, lru_iterator> key_finder;
    };
}

#endif 
