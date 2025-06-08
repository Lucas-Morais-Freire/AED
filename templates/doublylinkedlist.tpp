#include <doublylinkedlist.h>

#include <iostream>
#include <exception>

template <typename T>
typename DoublyLinkedList<T>::node* DoublyLinkedList<T>::allocate_new_node(T data) {
    node* new_node = new node;
    IF_DEBUG(std::cout << "allocated " << new_node << '\n');
    new_node->data = data;
    new_node->next = nullptr;
    new_node->prev = nullptr;

    return new_node;
}

template <typename T>
DoublyLinkedList<T>::~DoublyLinkedList() {
    while (head != nullptr) {
        node* temp = head;
        head = head->next;
        IF_DEBUG(std::cout << "deallocated " << temp << '\n');
        delete temp;
    }
}

template <typename T>
void DoublyLinkedList<T>::append(T data) {
    if (tail == nullptr) {
        tail = allocate_new_node(data);
        head = tail;
    } else {
        node* new_node = allocate_new_node(data);
        new_node->prev = tail;
        tail = new_node;
        new_node->prev->next = new_node;
    }
    n++;
}

template <typename T>
void DoublyLinkedList<T>::prepend(T data) {
    if (head == nullptr) {
        head = allocate_new_node(data);
        tail = head;
    } else {
        node* new_node = allocate_new_node(data);
        new_node->next = head;
        head = new_node;
        new_node->next->prev = new_node;
    }
    n++;
}

template <typename T>
bool DoublyLinkedList<T>::insert(T data, size_t pos) {
    if (pos > n) {
        return false;
    } else if (pos == 0) {
        prepend(data);
    } else if (pos == n) {
        append(data);
    } else {
        node* new_node = allocate_new_node(data);

        // navigate. Land exactly on the right position.
        node* temp;
        if (pos <= n/2) {
            temp = head;
            for (size_t i = 0; i < pos; i++)
                temp = temp->next;
        } else {
            temp = tail;
            for (size_t i = n - 1; i > pos; i--)
                temp = temp->prev;   
        }

        new_node->prev = temp->prev;
        new_node->next = temp;
        new_node->prev->next = new_node;
        new_node->next->prev = new_node; 

        n++;
    }

    return true;
}
template <typename T>
T& DoublyLinkedList<T>::operator[](size_t pos) {
    if (pos >= n) {
        throw GERROR("Invalid position: " + std::to_string(pos));
    }

    // navigate. Land exactly on the right position.
    node* temp;
    if (pos <= n/2) {
        temp = head;
        for (size_t i = 0; i < pos; i++)
            temp = temp->next;
    } else {
        temp = tail;
        for (size_t i = n - 1; i > pos; i--)
            temp = temp->prev;
    }

    return temp->data;
}

template <typename T>
bool DoublyLinkedList<T>::remove(size_t pos) {
    if (pos >= n) {
        return false;
    } else if (n == 1) {
        IF_DEBUG(std::cout << "deallocated " << head << '\n');
        delete head;
        head = nullptr;
        tail = nullptr;
    } else if (pos == 0) {
        head = head->next;
        IF_DEBUG(std::cout << "deallocated " << head->prev << '\n');
        delete head->prev;
        head->prev = nullptr;
    } else if (pos == n - 1) {
        tail = tail->prev;
        IF_DEBUG(std::cout << "deallocated " << tail->next << '\n');
        delete tail->next;
        tail->next = nullptr;
    } else {
        node* temp;

        // Navigate. Land exactly on the right position.
        if (pos <= n/2) {
            temp = head->next;
            for (size_t i = 1; i < pos; i++)
                temp = temp->next;
        } else {
            temp = tail->prev;
            for (size_t i = n - 2; i > pos; i--)
                temp = temp->prev;
        }

        temp->prev->next = temp->next;
        temp->next->prev = temp->prev;
        IF_DEBUG(std::cout << "deallocated " << temp << '\n');
        delete temp;
    }

    n--;
    return true;
}

template <typename T>
void DoublyLinkedList<T>::print() {
    if (head == nullptr) {
        std::cout << "()\n";
    } else {
        std::cout << "(" << head->data << ")";
        
        node* temp = head->next;
        while (temp != nullptr) {
            std::cout << "<->(" << temp->data << ")";
            temp = temp->next;
        }

        std::cout << '\n';
    }
}

template <typename T>
void DoublyLinkedList<T>::print_r() {
    if (head == nullptr) {
        std::cout << "()\n";
    } else {
        std::cout << "(" << tail->data << ")";
        
        node* temp = tail->prev;
        while (temp != nullptr) {
            std::cout << "<->(" << temp->data << ")";
            temp = temp->prev;
        }

        std::cout << '\n';
    }
}