#pragma once

#include <cstddef>

template <typename T>
class ArrayStack {
private:
    T* data;
    size_t _size;
    size_t _capacity;

    void expand();
public:
    ArrayStack(size_t size);
    ArrayStack() : ArrayStack(0) {}
    ~ArrayStack();

    // CRUD

    void push(T item); // (C)  put on stack
    T& peek();         // (RU) look at stack's top
    void remove();     // (D)  remove from stack
    T pop();           // (RD) take from stack
    size_t size() { return _size; }
    size_t capacity() { return _capacity; }

    void print();
};

#include <arraystack.tpp>