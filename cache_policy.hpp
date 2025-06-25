#ifndef CACHE_POLICY_HPP
#define CACHE_POLICY_HPP

#include <unordered_set>
#include <stdexcept>  // for std::runtime_error

namespace caches
{
    /*
     * Abstract cache policy interface for managing keys.
     */
    template <typename Key>
    class ICachePolicy
    {
    public:
        virtual ~ICachePolicy() = default;

        virtual void Insert(const Key& key) = 0;
        virtual void Touch(const Key& key) = 0;
        virtual void Erase(const Key& key) = 0;
        virtual const Key& ReplacementCandidate() const = 0;
    };

    /*
     * A dummy policy that doesn't follow any eviction rule.
     * Internally stores keys in an unordered set.
     * Picks any key from the set to evict — not based on order.
     */
    template <typename Key>
    class NoCachePolicy : public ICachePolicy<Key>
    {
    public:
        NoCachePolicy() = default;
        ~NoCachePolicy() noexcept override = default;

        void Insert(const Key& key) override {
            key_storage.emplace(key);
        }

        void Touch(const Key& key) noexcept override {
            // No effect in this policy
            (void)key;
        }

        void Erase(const Key& key) noexcept override {
            key_storage.erase(key);
        }

        const Key& ReplacementCandidate() const override {
            if (key_storage.empty()) {
                throw std::runtime_error("No keys available for replacement.");
            }
            return *key_storage.begin(); // any key is fine in no-policy
        }

    private:
        std::unordered_set<Key> key_storage;
    };

} // namespace caches

#endif 