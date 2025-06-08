#pragma once
#include <cstddef>

template <typename T>
class LinkedList {
private:
    struct node {
        T data;
        node* next;
    };

    size_t n;
    node* head;

    node* allocate_new_node(T data);
public:    
    LinkedList() : n(0), head(nullptr) {}
    ~LinkedList();
    size_t size() { return n; }

    // CRUD

    void append(T data);             // (C) add to end
    void prepend(T data);            // (C) add to beginning
    bool insert(T data, size_t pos); // (C) add to position
    T& operator[](size_t pos);       // (RU) get element at position
    bool remove(size_t pos);         // (D) delete element at position

    void print();
};

#include <linkedlist.tpp>