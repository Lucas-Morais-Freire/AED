#pragma once

#include <linkedlist.h>

template <typename T>
class LinkedStack : protected LinkedList<T> {
protected:
    using LinkedList<T>::prepend;
    using LinkedList<T>::remove;
    using LinkedList<T>::head;
    using typename LinkedList<T>::node;
    using LinkedList<T>::n;
public:
    // CRUD

    void push(T item) { prepend(item); }  // (C)  put on stack
    T& peek() { return head->data; }      // (RU) look at stack's top
    bool remove() { return remove(0); } // (D)  remove from stack
    T pop() {
        T item = head->data;
        remove(0);
        return item;
    } // (RD) take from stack
    size_t size() { return n; }

    void print() {
        if (n == 0) {
            std::cout << "(|\n";
        }

        std::cout << '(' << head->data;
        for (node* temp = head->next; temp != nullptr; temp = temp->next) {
            std::cout << ", " << temp->data;
        }
        std::cout << "|\n";
    }
};