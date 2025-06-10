#include <avltree.h>

#include <iostream>
#include <string>

template <typename T>
typename AVLTree<T>::node* AVLTree<T>::allocate_new_node(T data) {
    node* new_node = new node;
    new_node->data = data;
    new_node->height = 0;
    new_node->balance = 0;
    new_node->left = nullptr;
    new_node->right = nullptr;

    return new_node;
}

template <typename T>
void AVLTree<T>::update_height(typename AVLTree<T>::node* item) {
    int height_left  = item->left == nullptr ? 0 : item->left->height;
    int height_right = item->right == nullptr ? 0 : item->right->height;

    item->height = max(height_left, height_right) + 1;
}

template <typename T>
void AVLTree<T>::update_balance(typename AVLTree<T>::node* item) {
    int height_left  = item->left == nullptr ? 0 : item->left->height;
    int height_right = item->right == nullptr ? 0 : item->right->height;

    item->balance = height_right - height_left;
}

// recursive methods

template <typename T>
bool AVLTree<T>::insert_rec(typename AVLTree<T>::node* item, T data) {
    if (data < item->data) {
        if (item->left == nullptr) {
            item->left = allocate_new_node(data);
            update_height(item);
            update_balance(item);
            _size++;

            return true;
        } else if (insert_rec(item->left, data)) {
            update_height(item);
            update_balance(item);

            return true;
        }
    } else if (data > item->data) {
        if (item->right == nullptr) {
            item->right = allocate_new_node(data);
            update_height(item);
            update_balance(item);
            _size++;

            return true;
        } else if (insert_rec(item->right, data)) {
            update_height(item);
            update_balance(item);

            return true;
        }
    }

    return false;
}

template <typename T>
void AVLTree<T>::destroy_rec(node* item) {
    if (item->left != nullptr) destroy_rec(item->left);
    if (item->right != nullptr) destroy_rec(item->right);
    
    delete item;
}
template <typename T>
void AVLTree<T>::print_in_order_rec(typename AVLTree<T>::node* item) {
    if (item != nullptr) {
        print_in_order_rec(item->left);    
        std::cout << item->data << ", ";
        print_in_order_rec(item->right);    
    }
}

template <typename T>
void AVLTree<T>::print_rec(node *item, size_t offset) {
    std::string padding(offset, ' ');
    if (item->right != nullptr) {
        print_rec(item->right, offset + 4);
    } else {
        std::cout << padding + std::string(4, ' ') + "$\n";
    }
    std::cout << padding << item->data << '\n';
    if (item->left != nullptr) {
        print_rec(item->left, offset + 4);
    } else {
        std::cout << padding + std::string(4, ' ') + "$\n";
    }
}

template <typename T>
AVLTree<T>::~AVLTree() {
    if (_root != nullptr) destroy_rec(_root);
}

template <typename T>
bool AVLTree<T>::insert(T data) {
    if (_root == nullptr) {
        _root = allocate_new_node(data);
        _size++;
        return true;
    }

    return insert_rec(_root, data);
}

template <typename T>
void AVLTree<T>::insert(const T *data, size_t n) {
    for (size_t i = 0; i < n; i++) {
        insert(data[i]);
    }
}

template <typename T>
void AVLTree<T>::print() {
    print_rec(_root, 0);
}

template <typename T>
void AVLTree<T>::print_in_order()
{
    if (_root == nullptr) {
        std::cout << "<>\n";
        return;
    }

    std::cout << "<";
    print_in_order_rec(_root);
    std::cout << "\b\b>\n";
}