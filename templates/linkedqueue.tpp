#include <linkedqueue.h>

#include <iostream>

template <typename T>
inline typename LinkedQueue<T>::node* LinkedQueue<T>::allocate_new_node(T data) {
    node* new_node = new node;
    new_node->data = data;
    new_node->next = nullptr;
    return new_node;
}

template <typename T>
LinkedQueue<T>::~LinkedQueue() {
    while (_back != nullptr) {
        node* next = _back->next;
        delete _back;
        _back = next;
    }
}

template <typename T>
void LinkedQueue<T>::enqueue(T data) {
    if (_size == 0) {
        _back = allocate_new_node(data);
        _front = _back;
        _size++;
        return;
    }

    node* new_node = allocate_new_node(data);

    _back->next = new_node;
    _back = new_node;
    _size++;
}

template <typename T>
T LinkedQueue<T>::dequeue() {
    if (_size == 0) throw GERROR("This queue is already empty.");

    T item = _front->data;
    node* temp = _front->next;
    delete _front;
    _front = temp;
    _size--;
    return item;
}

template <typename T>
T LinkedQueue<T>::peek() {
    if (size == 0) throw GERROR("This queue is already empty.");

    return _front->data;
}

template <typename T>
void LinkedQueue<T>::print() const {
    if (_size == 0) {
        std::cout << "<-[]<-\n";
        return;
    }

    std::cout << "<-[" << _front->data;
    for (node* temp = _front->next; temp != nullptr; temp = temp->next) {
        std::cout << ", " << temp->data;
    }
    std::cout << "]<-\n";
}