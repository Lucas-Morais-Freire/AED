#include <hashtablenaive.h>
#include <iostream>

template <typename K, typename V, typename H>
HashTableNaive<K, V, H>::HashTableNaive(size_t size) : _cap(size), _size(0) {
    if (size == 0) {
        puts("Hash tables of size 0 are invalid. Creating one of size 8 by default.");
        _cap = 8;
    }
    _data = static_cast<slot*>(malloc(_cap*sizeof(slot)));
    for (size_t i = 0; i < _cap; i++) {
        _data[i].occupied = false;
    }
}

template <typename K, typename V, typename H>
HashTableNaive<K, V, H>::~HashTableNaive() {
    for (size_t i = 0; i < _cap; i++) {
        if (_data[i].occupied) {
            _data[i].value.~V();
            _data[i].key.~K();
        }
    }
    free(_data);
}

template <typename K, typename V, typename H>
template <typename X, typename Y>
void HashTableNaive<K, V, H>::insert(X&& key, Y&& value) {
    size_t i = get_idx(key);
    if (!_data[i].occupied) {
        new (&_data[i].key) K(std::forward<X>(key));
        new (&_data[i].value) V(std::forward<Y>(value));
        _data[i].occupied = true;
        _size++;
    } else {
        _data[i].key = std::forward<X>(key);
        _data[i].value = std::forward<Y>(value);
    }
}

template <typename K, typename V, typename H>
template <typename X, typename... Args>
void HashTableNaive<K, V, H>::grow(X &&key, Args&& ...args) {
    size_t i = get_idx(key);
    
    if (!_data[i].occupied) {
        new (&_data[i].key) K(std::forward<X>(key));
        new (&_data[i].value) V(std::forward<Args>(args)...);
        _size++;
        _data[i].occupied = true;
    } else {
        _data[i].key.~K();
        _data[i].value.~V();
        new (&_data[i].key) K(std::forward<X>(key));
        new (&_data[i].value) V(std::forward<Args>(args)...);
    }
}

template <typename K, typename V, typename H>
bool HashTableNaive<K, V, H>::contains(const K& key) {
    size_t i = get_idx(key);

    return _data[i].occupied ? true : false;
}

template <typename K, typename V, typename H>
V& HashTableNaive<K, V, H>::get(const K& key) {
    size_t i = get_idx(key);

    if (!_data[i].occupied) throw GERROR("This key was not found inside the table.");
    
    return _data[i].value;
}

template <typename K, typename V, typename H>
bool HashTableNaive<K, V, H>::erase(const K& key) {
    size_t i = get_idx(key);
    if (_data[i].occupied) {
        _data[i].key.~K();
        _data[i].value.~V();
        _size--;
        _data[i].occupied = false;
        return true;
    } else {
        return false;
    }
}

template <typename K, typename V, typename H>
V HashTableNaive<K, V, H>::take(const K& key) {
    size_t i = get_idx(key);

    if (!_data[i].occupied) throw GERROR("This key was not found in the hash table.");

    V ret(std::move(_data[i].value));
    _data[i].key.~K();
    _data[i].value.~V();
    _data[i].occupied = false;
    _size--;
    return ret;
}

template <typename K, typename V, typename H>
void HashTableNaive<K, V, H>::print() const {
    if constexpr(is_printable_v<K> && is_printable_v<V>) {
        std::cout << '{';
        bool print_comma = false;
        for (size_t i = 0; i < _cap; i++) {
            if (_data[i].occupied) {
                if (print_comma) {
                    std::cout << ", ";
                } else {
                    print_comma = true;
                }

                std::cout << _data[i].key << ": " << _data[i].value;
            }
        }
        std::cout << "}\n";
    } else {
        puts("This table is not printable.");
    }
}

template <typename K, typename V, typename H>
void HashTableNaive<K, V, H>::inner_print() const {
    if constexpr(is_printable_v<K> && is_printable_v<V>) {
        std::cout << '[';
        bool print_comma = false;
        for (size_t i = 0; i < _cap; i++) {
            if (print_comma) {
                std::cout << ", ";
            } else {
                print_comma = true;
            }

            if (_data[i].occupied) {
                std::cout << _data[i].key << ": " << _data[i].value;
            } else {
                std::cout << " ";
            }
        }
        std::cout << "]\n";
    } else {
        std::cout << "Either keys or values are not printable.\n";
    }
}