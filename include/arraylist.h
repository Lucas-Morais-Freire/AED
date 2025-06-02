#pragma once
#include <cstdlib>

template <typename T>
class ArrayList {
protected:
    T* data;
    size_t size;
    size_t capacity;

    void expand();
public:
    // constructors
    ArrayList(size_t capacity);
    ArrayList() : ArrayList(0) {};
    ArrayList(const ArrayList& other);
    ArrayList(ArrayList&& other) noexcept;

    // destructor
    ~ArrayList();

    // assignment operators  
    ArrayList& operator=(const ArrayList& other);
    ArrayList& operator=(ArrayList&& other) noexcept;
    
    // CRUD

    // Add item to end (move, copy)
    template <typename U>
    ArrayList& append(U&& obj);
    // Constructs item at end (append but create in-place)
    template <typename... Args>
    ArrayList& emplace(Args&&... args);
    // insert item at position (move, copy)
    template <typename U>
    ArrayList& insert(size_t i, U&& obj);
    // Constructs item at position (insert but create in-place)
    template <typename... Args>
    ArrayList& grow(size_t i, Args&&... args);
    // read, update
    T& operator[](size_t i);
    // remove (returns removed item)
    T remove(size_t i);
    // erase (simply destroys item)
    void erase(size_t i);

    // ostream interface
    template <typename U>
    friend std::ostream& operator<<(std::ostream& os, const ArrayList<U>& arr);
};

#include <arraylist.tpp>