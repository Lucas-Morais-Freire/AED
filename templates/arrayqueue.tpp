#include <arrayqueue.h>
#include <arraylist.h>

#include <iostream>
#include <string>

template <typename T>
void ArrayQueue<T>::expand() {
    T* new_data;
    if (_cap == 0) {
        new_data = new T[8];
    } else {
        new_data = new T[_cap << 1];
    }

    if constexpr(std::is_trivially_copyable_v<T>) {
        if (_top <= _bottom && _size != 0) {
            std::copy(_data + _bottom, _data + _size, new_data);
            std::copy(_data, _data + _top, new_data + _cap - _bottom);
        } else {
            std::copy(_data + _bottom, _data + _top, new_data);
        }
    } else {
        for (size_t i = 0; i < _size; i++) {
            new_data[i] = std::move(_data[(_bottom + i) % _cap]);
        }
    }

    _bottom = 0;
    _top = _size;
    delete[] _data;
    _data = new_data;
    _cap = _cap == 0 ? 8 : _cap << 1;
}

template <typename T>
ArrayQueue<T>::ArrayQueue(size_t capacity) : _data(nullptr), _bottom(0), _top(0), _size(0), _cap(capacity) {
    if (capacity != 0)
        _data = new T[capacity];
}

template <typename T>
ArrayQueue<T>::~ArrayQueue() {
    delete[] _data;
}

template <typename T>
void ArrayQueue<T>::enqueue(T item) {
    if (_size == _cap) expand();

    _data[_top] = item;
    _top = (_top + 1) % _cap;
    _size++;
}

template <typename T>
T ArrayQueue<T>::dequeue() {
    if (_size == 0) throw GERROR("Queue is already empty.");

    T item = _data[_bottom];
    _bottom = (_bottom + 1) % _cap;
    _size--;
    return item;
}

template <typename T>
T ArrayQueue<T>::peek() {
    return _data[_bottom];
}

template <typename T>
void ArrayQueue<T>::print() {
    if (_size == 0) {
        std::cout << "->[]->\n";
        return;
    }

    std::cout << "->[" << _data[_bottom];
    for (size_t i = (_bottom + 1) % _cap; i != _top; i = (i + 1) % _cap) {
        std::cout << ", " << _data[i];
    }
    std::cout << "]->\n";
}

template <typename T>
void ArrayQueue<T>::inner_print() {
    if (_cap == 0) {
        std::cout << "[]\n";
        return;
    }

    std::cout << "[";

    ArrayList<size_t> sizes(_size);

    bool write = _bottom < _top ? _bottom == 0 : _top != 0 || _bottom == 0;
    if (_size == 0) {
        write = false;
    }
    if (write) {
        std::cout << _data[0];
        sizes.emplace(std::to_string(_data[0]).size());
    } else {
        std::cout << " ";
        sizes.emplace(1);
    }

    for (size_t i = 1; i < _cap; i++) {
        if (i == _top) {
            write = !write;
        }
        if (i == _bottom) {
            write = !write;
        }

        if (write) {
            std::cout << ", " << _data[i];
            sizes.emplace(std::to_string(_data[i]).size());
        } else {
            std::cout << ",  ";
            sizes.emplace(1);
        }
    }

    std::cout << "]\n ";
    for (size_t i = 0; i < _cap; i++) {
        std::string a = "";
        for (size_t j = 0; j < sizes[i] - 1; j++) {
            a += " ";
        }

        if (i == _bottom && i == _top) {
            std::cout << "$  " + a;
        } else if (i == _bottom) {
            std::cout << "b  " + a;
        } else if (i == _top) {
            std::cout << "t  " + a;
        } else {
            std::cout << "   " + a;
        }
    }
    std::cout << '\n';
}