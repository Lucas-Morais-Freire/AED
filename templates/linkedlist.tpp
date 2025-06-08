#include <linkedlist.h>

#include <iostream>
#include <exception>

template <typename T>
LinkedList<T>::~LinkedList() {
    while (head != nullptr) {
        node* temp = head->next;
        delete head;
        head = temp;
    }
}

template <typename T>
typename LinkedList<T>::node* LinkedList<T>::allocate_new_node(T data) {
    node* new_node = new node;
    new_node->data = data;
    new_node->next = nullptr;
    return new_node;
}

template <typename T>
void LinkedList<T>::append(T data) {
    if (head == nullptr) {
        head = allocate_new_node(data);
        n++;
    } else {
        node* new_node = allocate_new_node(data);
        
        // for navigation
        node* temp = head;
        while (temp->next != nullptr) {
            temp = temp->next;
        }

        temp->next = new_node;
        n++;
    }
}

template <typename T>
void LinkedList<T>::prepend(T data) {
    node* new_node = allocate_new_node(data);
    new_node->next = head;
    head = new_node;
    n++;
}

template <typename T>
bool LinkedList<T>::insert(T data, size_t pos) {
    if (pos > n) {
        return false;
    } else if (pos == n) {
        append(data);
    } else if (pos == 0) {
        prepend(data);
    } else {
        node* new_node = allocate_new_node(data);

        node* temp = head;
        for (size_t i = 0; i < pos - 1; i++) {
            temp = temp->next;
        }

        new_node->next = temp->next;
        temp->next = new_node;
        n++;
    }

    return true;
}

template <typename T>
T& LinkedList<T>::operator[](size_t pos) {
    if (pos >= n) {
        throw std::runtime_error("Posição de acesso ilegal: " + std::to_string(pos));
    } else {
        node* temp = head;
        for (size_t i = 0; i < pos; i++) {
            temp = temp->next;
        }

        return temp->data;
    }
}

template <typename T>
bool LinkedList<T>::remove(size_t pos) {
    if (head == nullptr || pos >= n) {
        return false;
    } else if (pos == 0) {
        node* temp = head;
        head = head->next;
        delete temp;
        n--;
    } else {
        node* temp = head;
        for (size_t i = 0; i < pos - 1; i++) {
            temp = temp->next;
        }

        node* aux = temp->next;
        temp->next = temp->next->next;
        delete aux;
        n--;
    }
    
    return true;
}

template <typename T>
void LinkedList<T>::print() {
    if (n == 0) {
        std::cout << "()\n";
    } else {
        std::cout << "(" << head->data << ")";

        node* temp = head;
        while (temp->next != nullptr) {
            temp = temp->next;
            std::cout << "->(" << temp->data << ")";
        }
        std::cout << '\n';
    }
}