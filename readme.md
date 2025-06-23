# MemeCache

## _A minimal, header-only, in-memory caching library with basic eviction policies._

This library provides a simple in-memory cache implementation in C++ with support for three common cache eviction policies:

- **FIFO** (First-In/First-Out)
- **LIFO** (Last-In/Last-Out)
- **LRU** (Least Recently Used)

It is designed for educational and lightweight usage scenarios where a full-fledged cache system is not required.

---

## Features

- Simple C++ header-only library.
- Pluggable eviction policies via template.
- Easily extendable to support custom eviction strategies.
- STL-based: depends only on `std::unordered_map` and `std::list`.
- Easy to use with minimal setup.

---

## 📦 Included Cache Eviction Policies

- `fifo_cache_policy.hpp`
- `lifo_cache_policy.hpp`
- `lru_cache_policy.hpp`

Each policy can be used by simply including the relevant header and aliasing it via the provided template structure.

---

## 🚀 Usage Example

### LRU (Least Recently Used):

```cpp
#include "include/cache.hpp"
#include "include/lru_cache_policy.hpp"

template <typename Key, typename Value>
using lru_cache_t = caches::fixed_sized_cache<Key, Value, caches::LRUCachePolicy>;

int main() {
    constexpr std::size_t CACHE_SIZE = 256;
    lru_cache_t<char, int> cache(CACHE_SIZE);

    cache.Put('M', 24);
    cache.Put('P', 5);
    cache.Put('B', 2001);

    std::cout << "Using LRU Eviction Policy\n";
    std::cout << "Value for key 'M': " << cache.Get('M') << '\n';
    std::cout << "Value for key 'P': " << cache.Get('P') << '\n';
    std::cout << "Value for key 'B': " << cache.Get('B') << '\n';
}
```

### FIFO & LIFO usage can be similarly demonstrated:

```cpp
#include "include/fifo_cache_policy.hpp"
#include "include/lifo_cache_policy.hpp"

// aliasing
template <typename Key, typename Value>
using fifo_cache_t = caches::fixed_sized_cache<Key, Value, caches::FIFOCachePolicy>;

template <typename Key, typename Value>
using lifo_cache_t = caches::fixed_sized_cache<Key, Value, caches::LIFOCachePolicy>;
```

---

## 🛠️ Building & Running

Since the project is header-only, no library build step is required.

To compile and run:

```bash
g++ -std=c++17 main.cpp -o main
./main
```

> ⚠️ Make sure your system has a **C++17** compatible compiler (e.g., GCC 9+ or MSVC 2019+).

---

## 🧪 Sample Output

```
Hello Enterpret!
==============================================================================
Using FIFO Eviction Policy
Value for key '9': 81
Value for key '5': 25
==============================================================================
Using LIFO Eviction Policy
Value for key 'Backend': 40
Value for key 'intern': 4
==============================================================================
Using LRU Eviction Policy
Value for key 'M': 24
Value for key 'P': 5
Value for key 'B': 2001
==============================================================================
```

---

## 📂 Project Structure

```
├── include/
│   ├── cache.hpp                // core cache implementation
│   ├── cache_policy.hpp        // base policy interface
│   ├── fifo_cache_policy.hpp   // FIFO strategy
│   ├── lifo_cache_policy.hpp   // LIFO strategy
│   ├── lru_cache_policy.hpp    // LRU strategy
├── main.cpp                    // usage demo
├── README.md                   // this file
```

---

## ✏️ Extending with Custom Policies

To add a custom cache policy, create a class inheriting from `ICachePolicy<Key>` and implement the required methods:

```cpp
virtual void Insert(const Key& key) override;
virtual void Touch(const Key& key) override;
virtual void Erase(const Key& key) override;
virtual const Key& ReplacementCandidate() const override;
```

---

## ✅ Requirements

- A C++17 compatible compiler:
  - GCC ≥ 7
  - MSVC ≥ 2017
  - Clang ≥ 5

---

## 📄 License

This project is open-source and provided for learning and demonstration purposes. Feel free to modify or extend it for your own use.
