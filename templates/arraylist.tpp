// std
#include <iostream>

// 1st-party
#include "arraylist.h"
#include "google.h"

template <typename T>
void ArrayList<T>::expand() {
    T* new_data;
    if (capacity == 0) {
        new_data = new T[1];
    } else {
        new_data = new T[2*capacity];
    }
    
    if constexpr(std::is_trivially_copyable_v<T>) {
        std::copy(data, data + capacity, new_data);
    } else {
        for (size_t i = 0; i < capacity; i++) {
            new_data[i] = std::move(data[i]);
        }
    }

    delete[] data;
    data = new_data;
    capacity = capacity == 0 ? 1 : capacity << 1;
}

// standard constructor
template <typename T>
ArrayList<T>::ArrayList(size_t capacity) {
    CONSTRUCTOR("Standard constructor called!\n");
    data = new T[capacity];
    this->capacity = capacity;
    size = 0;
}

// copy constructor
template <typename T>
ArrayList<T>::ArrayList(const ArrayList<T>& other) {
    CONSTRUCTOR("Copy constructor called!\n");
    data = new T[other.capacity];

    capacity = other.capacity;
    size = other.size;

    for (size_t i = 0; i < other.size; i++) {
        data[i] = other.data[i];
    }
}

// move constructor
template <typename T>
ArrayList<T>::ArrayList(ArrayList<T>&& other) noexcept {
    CONSTRUCTOR("Move constructor called!\n");
    data = other.data;
    other.data = nullptr;
    size = other.size;
    other.size = 0;
    capacity = other.capacity;
    other.capacity = 0;
}

// copy assignment operator
template <typename T>
ArrayList<T>& ArrayList<T>::operator=(const ArrayList<T>& other) {
    CONSTRUCTOR(" Copy assignment called!\n");
    if (this != &other) {
        delete[] data;
        data = new T[other.capacity];

        capacity = other.capacity;
        size = other.size;

        for (size_t i = 0; i < other.size; i++) {
            data[i] = other.data[i];
        }
    }

    return *this;
}

// move assignment operator
template <typename T>
ArrayList<T>& ArrayList<T>::operator=(ArrayList<T>&& other) noexcept {
    CONSTRUCTOR("Move assignment called!\n");
    if (this != &other) {
        delete[] data;

        data = other.data;
        other.data = nullptr;

        size = other.size;
        capacity = other.capacity;

        other.size = 0;
        other.capacity = 0;
    }

    return *this;
}

template <typename T>
ArrayList<T>& ArrayList<T>::append(const T& obj) {
    if (size == capacity) expand();

    data[size++] = obj;

    return *this;
}

template <typename T>
ArrayList<T>& ArrayList<T>::append(T&& obj) {
    if (size == capacity) {
        expand();
    }

    data[size++] = std::forward<T>(obj);

    return *this;
}

template <typename T>
T &ArrayList<T>::operator[](size_t i)
{
    if (i >= this->size) {
        throw GERROR("Index " + std::to_string(i) + " is out of bounds.");
    }

    return data[i];
}

template <typename T>
T ArrayList<T>::remove(size_t i) {
    if (i >= size) {
        throw GERROR("Index " + std::to_string(i) + " is out of bounds.");
    }

    T temp = std::move(data[i]);

    for (size_t j = i; j < size - 1; j++) {
        data[j] = std::move(data[j + 1]);
    }
    
    size--;
    return temp;
}

// destructor
template <typename T>
ArrayList<T>::~ArrayList() {
    CONSTRUCTOR("Destructor called!\n");
    delete[] data;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const ArrayList<T>& arr) {
    std::string a;
    IF_DEBUG(os << "cap: " + std::to_string(arr.capacity) + " size: " + std::to_string(arr.size) + " ";)
    os << "[";
    if (arr.size == 0) {
        os << ']';
        return os;
    }

    os << arr.data[0];
    for (size_t i = 1; i < arr.size; i++) {
        os << ", " << arr.data[i];
    }
    os << "]";

    return os;
}