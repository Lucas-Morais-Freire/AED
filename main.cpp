// std
#include <iostream>
#include <array>
#include <random>
#include <unordered_map>

// 1st-party
#include <arraylist.h>
#include <linkedlist.h>
#include <doublylinkedlist.h>
#include <arraystack.h>
#include <linkedstack.h>
#include <arrayqueue.h>
#include <linkedqueue.h>
#include <avltree.h>
#include <hashtablenaive.h>
#include <hashtablechain.h>

struct p {
    size_t* v;
    size_t n;

    p() noexcept : v(nullptr), n(0) {
        CONSTRUCTOR("Empty constructor called!\n");
    };
    p(size_t n) noexcept : n(n) {
        CONSTRUCTOR("Standard constructor called!\n");

        v = new size_t[n];
        for (size_t i = 0; i < n; i++) {
            v[i] = i;
        }
    }

    p(const p& other) noexcept {
        CONSTRUCTOR("Copy constructor called!\n");
        v = new size_t[other.n];
        n = other.n;
        for (size_t i = 0; i < n; i++) {
            v[i] = other.v[i];
        }
    }

    p(p&& other) noexcept {
        CONSTRUCTOR("Move constructor called!\n");

        v = other.v;
        other.v = nullptr;
        n = other.n;
        other.n = 0;
    }

    p& operator=(const p& other) noexcept {
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
    
    p& operator=(p&& other) noexcept {
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

    ~p() noexcept {
        // std::cout << this << "\n";
        CONSTRUCTOR("Destructor called!\n");
    }
};

template <typename T>
struct myhash {size_t operator()(const T& s) {

    return 0;
}};


struct bruh {
    int x = 1;
    int y = 2;
    bruh(int x, int y) : x(x), y(y) {}
    bool operator==(const bruh& other) {
        if (x == other.x && y == other.y) return true;
        return false;
    }
};

namespace std {
    template <>
    struct hash<bruh> {
        size_t operator()(const bruh& b) const noexcept {
            size_t h1 = std::hash<int>()(b.x);
            size_t h2 = std::hash<int>()(b.y);
            return h1^(h2 << 1);
        }
    };
}

int main() {
    HashTableChain<bruh, p, std::hash<bruh>> table(10, 0.4);

    table.grow(bruh(1,2), 1);
    table.grow(bruh(2,3), 2);
    table.grow(bruh(-1,2), 3);
    table.grow(bruh(0,1), 4);
    std::cout << table << '\n';
    p yo = table.take(bruh(0,1));
    std::cout << table << '\n';
    std::cout << yo << '\n';
    // table.print();

    return 0;
}