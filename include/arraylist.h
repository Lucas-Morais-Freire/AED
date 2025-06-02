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
    ArrayList() : ArrayList(10) {};
    ArrayList(const ArrayList& other);
    ArrayList(ArrayList&& other) noexcept;

    // destructor
    ~ArrayList();

    // assignment operators  
    ArrayList& operator=(const ArrayList& other);
    ArrayList& operator=(ArrayList&& other) noexcept;
    
    // CRUD
    ArrayList& append(const T& obj);    // create
    ArrayList& append(T&& obj);         // move
    template <typename... Args>
    ArrayList& emplace(Args&&... args); // emplace
    T& operator[](size_t i);            // read, update
    T remove(size_t i);                 // remove
    void erase(size_t i);               // erase

    // ostream interface
    template <typename U>
    friend std::ostream& operator<<(std::ostream& os, const ArrayList<U>& arr);
};

#include <arraylist.tpp>

template <typename T>
template <typename... Args>
inline ArrayList<T>& ArrayList<T>::emplace(Args&& ...args) {
    if (size >= capacity) expand();
    new (&data[size++]) T(std::forward<Args>(args)...);

    return *this;
}
