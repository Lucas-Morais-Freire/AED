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
void AVLTree<T>::update_balance(typename AVLTree<T>::node* item) {
    if (item == nullptr) {
        return;
    } else if (!item->left && !item->right) {
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
    IF_DEBUG(
        if (item != nullptr) {
            std::cout << "rebalance called on " << item->data
                    << ", height: " << item->height
                    << ", bf: " << item->balance  << '\n';
        }
    );

    if (item == nullptr) {
        return nullptr;
    } else if (item->balance == -2 && item->left->balance == 1) {
        return rebalance_LR(item);
    } else if (item->balance == -2) {
        return rebalance_LL(item);
    } else if (item->balance == 2 && item->right->balance == -1) {
        return rebalance_RL(item);
    } else if (item->balance == 2) {
        return rebalance_RR(item);
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
        item->left = insert_rec(item->left, data);
        update_balance(item);
        return rebalance(item);
    } else if (data > item->data) {
        item->right = insert_rec(item->right, data);
        update_balance(item);
        return rebalance(item);
    } else {
        return item;
    }
}

template <typename T>
typename AVLTree<T>::node* AVLTree<T>::find_min(typename AVLTree<T>::node* item) {
    node* temp = item;
    while (temp->left != nullptr) {
        temp = temp->left;
    }

    return temp;
}

template <typename T>
typename AVLTree<T>::node *AVLTree<T>::remove_rec(node *item, T data)
{
    if (item == nullptr) {
        return nullptr;
    }

    if (data < item->data) {
        item->left = remove_rec(item->left, data);
    } else if (data > item->data) {
        item->right = remove_rec(item->right, data);
    } else {
        node* to_delete;
        if (!item->left && !item->right) {
            to_delete = item;
            item = nullptr;
        } else if (!item->left && item->right) {
            to_delete = item;
            item = item->right;
        } else if (item->left && !item->right) {
            to_delete = item;
            item = item->left;
        } else {
            to_delete = item;
            item = allocate_new_node(find_min(to_delete->right)->data);
            item->right = remove_rec(to_delete->right, item->data);
            item->left = to_delete->left;
        }
        
        delete to_delete;
        _size--;
    }

    update_balance(item);
    return rebalance(item);
}

template <typename T>
void AVLTree<T>::destroy_rec(node* item) {
    if (item->left != nullptr) destroy_rec(item->left);
    if (item->right != nullptr) destroy_rec(item->right);
    
    IF_DEBUG(std::cout << "deleting " << item->data << '\n');
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
void AVLTree<T>::pretty_print_rec(typename AVLTree<T>::node *item, size_t depth, bool* swaps, bool right,
                                  size_t branch_length) {
    if (item == nullptr) {
        return;
    }
    
    swaps[depth + 1] = right ? false : true;
    pretty_print_rec(item->right, depth + 1, swaps, true, branch_length);
    std::string padding(branch_length, ' ');
    std::string branch;
    for (size_t i = 0; i < branch_length; i++)
        branch += "\u2501";
    for (size_t i = 2; i <= depth; i++) {
        if (swaps[i]) {
            std::cout << " \u2503" + padding;
        } else {
            std::cout << "  " + padding;
        }
    }

    if (right) {
        std::cout << " \u250f" + branch;
    } else {
        std::cout << " \u2517" + branch;
    }
    std::cout << "[" << item->data << "]\n";
    
    swaps[depth + 1] = !swaps[depth + 1];
    pretty_print_rec(item->left, depth + 1, swaps, false, branch_length);
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

    size_t prev_size = _size;
    _root = insert_rec(_root, data);
    return prev_size != _size ? true : false;
}

template <typename T>
void AVLTree<T>::insert(const T *data, size_t n) {
    for (size_t i = 0; i < n; i++) {
        IF_DEBUG(
            for (size_t j = 0; j < n; j++) {
                if (i == j) std::cout << "\033[31m";
                std::cout << data[j];
                if (i == j) std::cout << "\033[37m";
            }
            std::cout << '\n';
        );
        insert(data[i]);
        IF_DEBUG(pretty_print());
    }
}

template <typename T>
bool AVLTree<T>::find(T data) {
    node* temp = _root;
    while (temp != nullptr) {
        if (data == temp->data) {
            return true;
        } else if (data > temp->data) {
            temp = temp->right;
        } else {
            temp = temp->left;
        }
    }

    return false;
}

template <typename T>
bool AVLTree<T>::remove(T data) {
    if (_root == nullptr) {
        return false;
    }

    size_t prev_size = _size;
    _root = remove_rec(_root, data);
    return prev_size != _size ? true : false;
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
void AVLTree<T>::pretty_print(size_t branch_length) {
    bool* swaps = new bool[_root->height + 2];
    swaps[0] = false;
    swaps[1] = false;

    pretty_print_rec(_root->right, 1, swaps, true, branch_length);
    std::cout << "[" << _root->data << "]\n";
    pretty_print_rec(_root->left, 1, swaps, false, branch_length);

    delete[] swaps;
}
