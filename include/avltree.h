#pragma once
#include <cstddef>

template <typename T>
class AVLTree {
private:
    struct node {
        T data;
        node* left;
        node* right;
        int height;
        int balance;
    };

    node* _root;
    size_t _size;

    node* allocate_new_node(T item);

    size_t max(size_t x, size_t y) { return x > y ? x : y; }

    void update_height(node* item);
    void update_balance(node* item);

    node* rotate_left(node* item);
    node* rotate_right(node* item);

    node* rebalance_LL(node* item);
    node* rebalance_LR(node* item);
    node* rebalance_RL(node* item);
    node* rebalance_RR(node* item);
    node* rebalance(node* item);
    
    bool insert_rec(node* item, T data);
    void destroy_rec(node* item);
    void print_in_order_rec(node* item);
    void print_rec(node* item, size_t offset);
public:
    AVLTree() : _root(nullptr), _size(0) {}
    ~AVLTree();

    bool insert(T data);
    void insert(const T* data, size_t n);
    bool find(T data);
    bool remove(T data);

    void print();
    void print_in_order();
};

#include <avltree.tpp>