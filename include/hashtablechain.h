#pragma once
#include <unordered_map>
#include <type_traits>

template <typename K, typename V, typename H = std::hash<K>>
class HashTableChain {
private:
    // any given type is false type by default
    template <typename T, typename = void>
    struct is_printable : std::false_type {};

    // specialize for void_t if std::ostream&() << T() operator compiles. If so, inherit true type
    template <typename T>
    struct is_printable<
        T, 
        std::void_t<
            decltype(std::declval<std::ostream&>() << std::declval<T>())
        >
    > : std::true_type {};

    template <typename T>
    static bool constexpr is_printable_v = is_printable<T>::value;

    struct slot {
        K key;
        V value;
        slot* next;
    };

    slot** _data;
    size_t _size;
    size_t _cap;
    float _lf;
    float _rt;
    H _hash;
    static constexpr bool _printable = is_printable_v<K> && is_printable_v<V>;

    size_t get_idx(const K& key) { return _hash(key) % _cap; }
    void update_lf() { _lf = static_cast<float>(_size)/_cap; }
    void rehash();
public:
    /**
     * \brief Construct a hash table.
     * \param capacity The initial capacity of the hash table.
     * \param rehash_threshold The hash table will rehash when the
     * load factor is greater than or equal to this.
     */
    HashTableChain(size_t capacity, float rehash_threshold = 0.5);
    HashTableChain() : HashTableChain(8) {}
    ~HashTableChain();

    // CRUD

    template <typename Kt, typename Vt>
    void insert(Kt&& key, Vt&& value);       // (C) insert a new value
    template <typename Kt, typename... Args>
    void grow(Kt&& key, Args&&... args);     // (C) construct a value in place
    V& get(const K& key);                    // (RU) get value from key
    bool contains(const K& key);
    bool erase(const K& key);
    V take(const K& key);

    size_t size() const noexcept { return _size; }
    size_t capacity() const noexcept { return _cap; }
    float load_factor() const noexcept { return _lf; }

    void print();
    void inner_print();

    template <typename Kt, typename Vt, typename Ht>
    friend std::ostream& operator<<(std::ostream& os, const HashTableChain<Kt, Vt, Ht>& table);
};

#include <hashtablechain.tpp>