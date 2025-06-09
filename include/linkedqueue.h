#pragma once

#include <cstddef>

template <typename T>
class LinkedQueue {
private:
    struct node {
        T data;
        node* next;
    };

    node* _back;
    node* _front;
    size_t _size;

    node* allocate_new_node(T data);
public:
    LinkedQueue() : _back(nullptr), _front(nullptr), _size(0) {}
    ~LinkedQueue();

    size_t size() const { return _size; }

    // CRUD

    void enqueue(T data);
    T dequeue();
    T peek();
    void print() const;
};

#include <linkedqueue.tpp>