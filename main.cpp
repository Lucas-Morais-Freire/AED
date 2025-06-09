// std
#include <iostream>
#include <array>

// 1st-party
#include <arraylist.h>
#include <linkedlist.h>
#include <doublylinkedlist.h>
#include <arraystack.h>
#include <linkedstack.h>
#include <arrayqueue.h>

struct p {
    size_t* v;
    size_t n;

    p() : v(nullptr), n(0) {
        CONSTRUCTOR("Empty constructor called!\n");
    };
    p(size_t n) : n(n) {
        CONSTRUCTOR("Standard constructor called!\n");

        v = new size_t[n];
        for (size_t i = 0; i < n; i++) {
            v[i] = i;
        }
    }

    p(const p& other) {
        CONSTRUCTOR("Copy constructor called!\n");
        v = new size_t[other.n];
        n = other.n;
        for (size_t i = 0; i < n; i++) {
            v[i] = other.v[i];
        }
    }

    p(p&& other) {
        CONSTRUCTOR("Move constructor called!\n");

        v = other.v;
        other.v = nullptr;
        n = other.n;
        other.n = 0;
    }

    p& operator=(const p& other) {
        CONSTRUCTOR("Copy assignment called!\n");

        if (this != &other) {
            n = other.n;
            delete[] v;
            v = new size_t[n];

            for (size_t i = 0; i < n; i++) {
                v[i] = other.v[i];
            }
        }

        return *this;
    }
    
    p& operator=(p&& other) {
        CONSTRUCTOR("Move assignment called!\n");

        if (this != &other) {
            delete[] v;
            v = other.v;
            other.v = nullptr;
            n = other.n;
            other.n = 0;
        }

        return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, const p& obj) {
        if (obj.n == 0) {
            return os << "[]";
        }

        os << '[' + std::to_string(obj.v[0]);
        for (size_t i = 1; i < obj.n; i++) {
            os <<  ", " + std::to_string(obj.v[i]);
        }
        return os << ']';
    }

    ~p() {
        // std::cout << this << "\n";
        CONSTRUCTOR("Destructor called!\n");
    }
};

int main() {
    ArrayQueue<int> queue(1);

    queue.enqueue(-1);
    queue.enqueue(-2);
    queue.dequeue();
    queue.dequeue();
    queue.enqueue(-30);
    queue.enqueue(-32);
    queue.enqueue(-42);
    queue.dequeue();
    queue.enqueue(54);
    queue.enqueue(-6);
    queue.inner_print();
    queue.enqueue(-90);
    queue.inner_print();
    while (queue.size() > 0) {
        queue.dequeue();
    }
    queue.enqueue(1);
    queue.inner_print();

    return 0;
}