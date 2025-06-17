#include <hashtablechain.h>

#include <iostream>

template <typename K, typename V, typename H>
void HashTableChain<K, V, H>::rehash() {
    size_t old_cap = _cap;
    _cap = _cap << 1;

    slot** new_data = new slot*[_cap]();

    for (size_t i = 0; i < old_cap; i++) {
        slot* temp = _data[i];
        slot* next;
        while (temp != nullptr) {
            next = temp->next;
            size_t j = get_idx(temp->key);
            temp->next = new_data[j];
            new_data[j] = temp;
            temp = next;
        }
    }

    update_lf();
    delete[] _data;
    _data = new_data;
}

template <typename K, typename V, typename H>
HashTableChain<K, V, H>::HashTableChain(size_t capacity, float rehash_threshold) :
    _size(0), _cap(capacity), _lf(0), _rt(rehash_threshold) {
    _data = new slot*[_cap]();
}

template <typename K, typename V, typename H>
HashTableChain<K, V, H>::~HashTableChain() {
    for (size_t i = 0; i < _cap; i++) {
        slot* to_delete = _data[i];
        slot* next;
        while (to_delete != nullptr) {
            next = to_delete->next;
            to_delete->key.~K();
            to_delete->value.~V();
            free(to_delete);
            to_delete = next;
        }
    }
    delete[] _data;
}

template <typename K, typename V, typename H>
template <typename Kt, typename Vt>
void HashTableChain<K, V, H>::insert(Kt&& key, Vt&& value) {
    size_t i = get_idx(key);
    slot* temp = _data[i];
    while (temp != nullptr) {
        if (key == temp->key) {
            temp->value = std::forward<Vt>(value);
            return;
        }
        temp = temp->next;
    }

    if (_lf >= _rt || _cap == _size) rehash();

    slot* new_slot = static_cast<slot*>(malloc(sizeof(slot)));
    new (&new_slot->key) K(std::forward<Kt>(key));
    new (&new_slot->value) V(std::forward<Vt>(value));

    new_slot->next = _data[i];
    _data[i] = new_slot;
    _size++;

    update_lf();
}

template <typename K, typename V, typename H>
template <typename Kt, typename... Args>
void HashTableChain<K, V, H>::grow(Kt &&key, Args&&... args) {
    size_t i = get_idx(key);
    slot* temp = _data[i];
    while (temp != nullptr) {
        if (key == temp->key) {
            temp->value = V(std::forward<Args>(args)...);
            return;
        }
        temp = temp->next;
    }

    if (_lf >= _rt || _cap == _size) rehash();

    slot* new_slot = static_cast<slot*>(malloc(sizeof(slot)));
    new (&new_slot->key) K(std::forward<Kt>(key));
    new (&new_slot->value) V(std::forward<Args>(args)...);

    new_slot->next = _data[i];
    _data[i] = new_slot;
    _size++;

    update_lf();
}

template <typename K, typename V, typename H>
bool HashTableChain<K, V, H>::contains(const K& key) {
    size_t i = get_idx(key);

    slot* temp = _data[i];
    while (temp != nullptr) {
        if (temp->key == key) {
            return true;
        }
        temp = temp->next;
    }

    return false;
}

template <typename K, typename V, typename H>
V& HashTableChain<K, V, H>::get(const K& key) {
    size_t i = get_idx(key);

    slot* temp = _data[i];
    while (temp != nullptr) {
        if (temp->key == key) {
            return temp->value;
        }
        temp = temp->next;
    }

    throw std::out_of_range("key not found");
}

template <typename K, typename V, typename H>
bool HashTableChain<K, V, H>::erase(const K& key) {
    size_t i = get_idx(key);

    slot* curr = _data[i];
    slot* prev = nullptr;
    while (curr != nullptr) {
        if (curr->key == key) {
            if (prev == nullptr) {
                _data[i] = curr->next;
            } else {
                prev->next = curr->next;
            }

            curr->key.~K();
            curr->value.~V();
            free(curr);

            _size--;
            update_lf();
            return true;
        }
        prev = curr;
        curr = curr->next;
    }

    return false;
}

template <typename K, typename V, typename H>
V HashTableChain<K, V, H>::take(const K& key) {
    size_t i = get_idx(key);

    slot* curr = _data[i];
    slot* prev = nullptr;
    while (curr != nullptr) {
        if (curr->key == key) {
            if (prev == nullptr) {
                _data[i] = curr->next;
            } else {
                prev->next = curr->next;
            }
            break;
        }
        prev = curr;
        curr = curr->next;
    }

    if (curr == nullptr) throw std::out_of_range("This key was not found.");

    V ret(std::move(curr->value));

    curr->key.~K();
    curr->value.~V();
    free(curr);

    _size--;
    update_lf();
    return ret;
}

template <typename K, typename V, typename H>
void HashTableChain<K, V, H>::print() {
    if constexpr(is_printable_v<K> && is_printable_v<V>) {
        std::cout << '{';
        bool print_comma = false;
        for (size_t i = 0; i < _cap; i++) {
            slot* curr = _data[i];
            while (curr != nullptr) {
                if (print_comma) {
                    std::cout << ", ";
                } else {
                    print_comma = true;
                }

                std::cout << curr->key << ": " << curr->value;
                curr = curr->next;
            }
        }
        std::cout << "}\n";
    } else {
        std::cout << "size: " << _size << '\n';
    }
}

template <typename K, typename V, typename H>
void HashTableChain<K, V, H>::inner_print() {

    if constexpr(is_printable_v<K> && is_printable_v<V>) {
        for (size_t i = 0; i < _cap; i++) {
            slot* temp = _data[i];
            bool print_arrow = false;
            std::cout << i << ": ";
            while (temp != nullptr) {
                if (print_arrow) {
                    std::cout << "->";
                } else {
                    print_arrow = true;
                }
                std::cout << '[' << temp->key << ": " << temp->value << ']';
                temp = temp->next;
            }
            std::cout << '\n';
        }
    } else {
        std::cout << "Either keys or values are not printable.\n";
    }
}

template <typename Kt, typename Vt, typename Ht>
std::ostream& operator<<(std::ostream &os, const HashTableChain<Kt, Vt, Ht>& table) {
    if constexpr(HashTableChain<Kt, Vt, Ht>::_printable) {
        os << '{';
        bool print_comma = false;
        for (size_t i = 0; i < table._cap; i++) {
            typename HashTableChain<Kt, Vt, Ht>::slot* curr = table._data[i];
            while (curr != nullptr) {
                if (print_comma) {
                    os << ", ";
                } else {
                    print_comma = true;
                }

                os << curr->key << ": " << curr->value;
                curr = curr->next;
            }
        }
        os << '}';
    } else {
        os << "size: " << table._size;
    }
    return os;
}
