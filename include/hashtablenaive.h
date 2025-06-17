#pragma once
#include <cstddef>
#include <unordered_map>

#include <type_traits>
#include <ostream>

template <typename K, typename V, typename H = std::hash<K>>
class HashTableNaive {
private:
    template <typename T, typename = void>
    struct is_printable : std::false_type {};

    template <typename T>
    struct is_printable<
        T,
        std::void_t<
            decltype(std::declval<std::ostream&>() << std::declval<T>())
        >
    > : std::true_type {};

    template <typename T>
    static constexpr bool is_printable_v = is_printable<T>::value;

    struct slot {
        K key;
        V value;
        bool occupied = false;
        ~slot() {
            key.~K();
            value.~V();
        }
    };
    slot* _data;
    size_t _cap;
    size_t _size;
    H hash;

    size_t get_idx(const K& key) { return hash(key) % _cap; }
public:
    HashTableNaive(size_t size);
    HashTableNaive() : HashTableNaive(8) {}
    ~HashTableNaive();
    template <typename X, typename Y>
    void insert(X&& key, Y&& value);
    template <typename X, typename... Args>
    void grow(X&& key, Args&&... args);
    bool contains(const K& key);
    V& get(const K& key);
    bool erase(const K& key);
    V take(const K& key);

    size_t size() const noexcept { return _size; }
    size_t capacity() const noexcept { return _cap; }
    
    void print() const;
    void inner_print() const;
};

#include <hashtablenaive.tpp>