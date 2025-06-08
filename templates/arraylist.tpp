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
        std::copy(data, data + size, new_data);
    } else {
        for (size_t i = 0; i < size; i++) {
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
    CONSTRUCTOR("Copy assignment called!\n");
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
template <typename U>
ArrayList<T>& ArrayList<T>::append(U&& obj) {
    if (size == capacity) expand();
    data[size++] = std::forward<U>(obj);

    return *this;
}

template <typename T>
template <typename... Args>
inline ArrayList<T>& ArrayList<T>::emplace(Args&& ...args) {
    if (size >= capacity) expand();
    data[size].~T();
    new (&data[size++]) T(std::forward<Args>(args)...);

    return *this;
}

template <typename T>
template <typename U>
ArrayList<T>& ArrayList<T>::insert(size_t i, U&& obj) {
    if (i > size) {
        throw GERROR("Index " + std::to_string(i) + " is out of bounds.");
    } else if (i == size) {
        append(std::forward<U>(obj));
        return *this;
    }

    if (size == capacity) expand();

    for (size_t j = size - 1; j >= i; j--) {
        data[j + 1] = std::move(data[j]);
    }

    data[i] = std::forward<U>(obj);
    size++;

    return *this;
}

template <typename T>
template <typename... Args>
ArrayList<T> &ArrayList<T>::grow(size_t i, Args&&... args) {
    if (i > size) {
        throw GERROR("Index " + std::to_string(i) + " is out of bounds.");
    } else if (i == size) {
        emplace(std::forward<Args>(args)...);
        return *this;
    }

    if (size == capacity) expand();

    for (size_t j = size - 1; j >= i; j--) {
        data[j + 1] = std::move(data[j]);
    }

    new (&data[i]) T(std::forward<Args>(args)...);
    size++;

    return *this;
}

template <typename T>
T &ArrayList<T>::operator[](size_t i) {
    if (i >= this->size) {
        throw GERROR("Index " + std::to_string(i) + " is out of bounds.");
    }

    return data[i];
}

template <typename T>
T ArrayList<T>::remove(size_t i) {
    if (i >= size) throw GERROR("Index " + std::to_string(i) + " is out of bounds.");

    T temp = std::move(data[i]);

    for (size_t j = i; j < size - 1; j++) {
        data[j] = std::move(data[j + 1]);
    }
    
    size--;
    return temp;
}

template <typename T>
void ArrayList<T>::erase(size_t i) {
    if (i >= size) throw GERROR("Index " + std::to_string(i) + " is out of bounds.");

    for (size_t j = i; j < size - 1; j++) {
        data[j] = std::move(data[j + 1]);
    }
    
    size--;
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