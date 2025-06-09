#pragma once

#include <cstddef>

template <typename T>
class ArrayQueue {
private:
    T* _data;
    size_t _bottom;
    size_t _top;
    size_t _size;
    size_t _cap;

    void expand();
public:
    ArrayQueue(std::size_t capacity);
    ArrayQueue() : ArrayQueue(0) {}
    ~ArrayQueue();

    size_t size() const { return _size; }
    size_t capacity() const { return _cap; }
    
    // CRUD

    void enqueue(T data); // (C)  add to queue 
    T dequeue();          // (RD) take from queue
    T peek();             // (R)  inspect front of queue
    void print();
    void inner_print();
};

#include <arrayqueue.tpp>