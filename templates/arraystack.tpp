#include <arraystack.h>

#include <iostream>
#include <exception>

template <typename T>
void ArrayStack<T>::expand() {
    T* new_data;
    if (_capacity == 0) {
        new_data = new T[8];
    } else {
        new_data = new T[2*_capacity];
    }

    if constexpr(std::is_trivially_copyable_v<T>) {
        std::copy(data, data + _size, new_data);
    } else {
        for (size_t i = 0; i < _size; i++) {
            new_data = std::move(data[i]);
        }
    }

    delete[] data;
    data = new_data;
    _capacity = _capacity == 0 ? 8 : _capacity << 1;
}

template <typename T>
ArrayStack<T>::ArrayStack(size_t size) : data(nullptr), _size(size), _capacity(size) {
    if (size != 0)
        data = new T[size];
}

template <typename T>
ArrayStack<T>::~ArrayStack() {
    delete[] data;
}

template <typename T>
void ArrayStack<T>::push(T item) {
    if (_size == _capacity) {
        expand();
    }

    data[_size++] = item;
}

template <typename T>
T& ArrayStack<T>::peek() {
    return data[_size - 1];
}

template <typename T>
void ArrayStack<T>::remove() {
    if (_size == 0) {
        throw GERROR("Stack is already empty.");
    }

    _size--;
}

template <typename T>
T ArrayStack<T>::pop() {
    if (_size == 0) {
        throw GERROR("Stack is already empty.");
    } else {
        return data[--_size];
    }
}

template <typename T>
void ArrayStack<T>::print() {
    if (_size == 0) {
        std::cout << "|)\n";
        return;
    }

    std::cout << "|" << data[0];

    for (size_t i = 1; i < _size; i++) {
        std::cout << ", " << data[i];
    }
    std::cout << ")\n";
}