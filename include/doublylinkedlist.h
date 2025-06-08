#pragma once

#include <cstddef>

template <typename T>
class DoublyLinkedList {
private:
    struct node {
        T data;
        node* prev;
        node* next;
    };

    node* head;
    node* tail;
    size_t n;

    node* allocate_new_node(T data);
public:
    DoublyLinkedList() : head(nullptr), tail(nullptr), n(0) {};
    ~DoublyLinkedList();

    size_t size() { return  n; }

    void append(T data);
    void prepend(T data);
    bool insert(T data, size_t pos);
    T& operator[](size_t pos);
    bool remove(size_t pos);

    void print();
    void print_r();
};

#include <doublylinkedlist.tpp>