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
void AVLTree<T>::update_balance(typename AVLTree<T>::node *item)
{
    if (!item->left && !item->right) {
        item->height = 0;
        item->balance = 0;
    } else if (!item->left && item->right) {
        item->height = item->right->height + 1;
        item->balance = item->right->height;
    } else if (item->left && !item->right) {
        item->height = item->left->height + 1;
        item->balance = -item->left->height;
    } else {
        item->height = max(item->right->height, item->left->height) + 1;
        item->balance = item->right->height - item->left->height;
    }
}

template <typename T>
typename AVLTree<T>::node* AVLTree<T>::rotate_left(typename AVLTree<T>::node* item) {
    node* right = item->right;
    item->right = right->left;
    right->left = item;

    update_balance(item);
    update_balance(right);

    return right;
}

template <typename T>
typename AVLTree<T>::node* AVLTree<T>::rotate_right(typename AVLTree<T>::node* item) {
    node* left = item->left;
    item->left = left->right;
    left->right = item;

    update_balance(item);
    update_balance(left);

    return left;
}

template <typename T>
typename AVLTree<T>::node* AVLTree<T>::rebalance_LL(typename AVLTree<T>::node* item) {
    return rotate_right(item);
}

template <typename T>
typename AVLTree<T>::node *AVLTree<T>::rebalance_LR(typename AVLTree<T>::node *item) {
    item->left = rotate_left(item->left);
    return rotate_right(item);
}

template <typename T>
typename AVLTree<T>::node* AVLTree<T>::rebalance_RR(typename AVLTree<T>::node* item) {
    return rotate_left(item);
}

template <typename T>
typename AVLTree<T>::node *AVLTree<T>::rebalance_RL(typename AVLTree<T>::node *item) {
    item->right = rotate_right(item->right);
    return rotate_left(item);
}

template <typename T>
typename AVLTree<T>::node *AVLTree<T>::rebalance(typename AVLTree<T>::node *item) {
    if (item->balance == -2 && item->left->balance == -1) {
        return rebalance_LL(item);
    } else if (item->balance == -2 && item->left->balance == 1) {
        return rebalance_LR(item);
    } else if (item->balance == 2 && item->right->balance == 1) {
        return rebalance_RR(item);
    } else if (item->balance == 2 && item->right->balance == -1) {
        return rebalance_RL(item);
    }

    return item;
}

// recursive methods

template <typename T>
typename AVLTree<T>::node *AVLTree<T>::insert_rec(typename AVLTree<T>::node *item, T data) {
    if (item == nullptr) {
        _size++;
        return allocate_new_node(data);
    }

    if (data < item->data) {
        node* temp = insert_rec(item->left, data);
        if (temp != nullptr) {
            item->left = temp;
            update_balance(item);
            return rebalance(item);
        }
    } else if (data > item->data) {
        node* temp = insert_rec(item->right, data);
        if (temp != nullptr) {
            item->right = temp;
            update_balance(item);
            return rebalance(item);
        }
    }

    return nullptr;
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
void AVLTree<T>::pretty_print_rec(typename AVLTree<T>::node *item, size_t depth, bool* swaps, bool right) {
    if (item == nullptr) {
        return;
    }
    
    swaps[depth + 1] = right ? false : true;
    pretty_print_rec(item->right, depth + 1, swaps, true);

    for (size_t i = 1; i < depth; i++) {
        if (swaps[i]) {
            std::cout << "|";
        } else {
            std::cout << " ";
        }
    }

    std::cout << '[' << item->data << "]\n";
    
    swaps[depth + 1] = !swaps[depth + 1];
    pretty_print_rec(item->left, depth + 1, swaps, false);
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
void AVLTree<T>::print_bf_rec(typename AVLTree<T>::node* item, size_t offset) {
    std::string padding(offset, ' ');
    if (item->right != nullptr) {
        print_bf_rec(item->right, offset + 4);
    } else {
        std::cout << padding + std::string(4, ' ') + "$\n";
    }
    std::cout << padding << item->height << ',' << item->balance << '\n';
    if (item->left != nullptr) {
        print_bf_rec(item->left, offset + 4);
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

    node* new_root = insert_rec(_root, data);
    if (new_root == nullptr) {
        return false;
    } else {
        _root = new_root;
        return true;
    }
}

template <typename T>
void AVLTree<T>::insert(const T *data, size_t n) {
    for (size_t i = 0; i < n; i++) {
        insert(data[i]);
        print();
        std::cout << "-----\n";
        print_bf();
        std::cout << "---------------------------\n";
    }
}

template <typename T>
void AVLTree<T>::print() {
    print_rec(_root, 0);
}

template <typename T>
void AVLTree<T>::print_bf() {
    print_bf_rec(_root, 0);
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

template <typename T>
void AVLTree<T>::pretty_print() {
    bool* swaps = new bool[_root->height + 2];
    swaps[0] = false;
    swaps[1] = false;

    pretty_print_rec(_root->right, 1, swaps, true);
    std::cout << "[" << _root->data << "]\n";
    pretty_print_rec(_root, 1, swaps, false);

    delete[] swaps;
}
