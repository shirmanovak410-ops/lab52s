//
// Created by Ксения on 07.03.2026.
//
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <iterator>

template <class T>
class vector {
private:
    size_t size;
    size_t capacity;
    T* data;

public:
    //итераторы
    class iterator : public std::iterator<std::random_access_iterator_tag, T> {
    private:
        T* ptr;
    public:
        iterator(T* p = nullptr) : ptr(p) {}
        iterator& operator++() { ++ptr; return *this; }
        iterator operator++(int) { iterator tmp(*this); ++ptr; return tmp; }
        iterator& operator--() { --ptr; return *this; }
        iterator operator--(int) { iterator tmp(*this); --ptr; return tmp; }
        iterator& operator+=(ptrdiff_t n) { ptr += n; return *this; }
        iterator& operator-=(ptrdiff_t n) { ptr -= n; return *this; }
        iterator operator+(ptrdiff_t n) const { return iterator(ptr + n); }
        iterator operator-(ptrdiff_t n) const { return iterator(ptr - n); }
        ptrdiff_t operator-(const iterator& other) const { return ptr - other.ptr; }
        T& operator*() const { return *ptr; }
        T* operator->() const { return ptr; }
        T& operator[](ptrdiff_t n) const { return ptr[n]; }
        bool operator==(const iterator& other) const { return ptr == other.ptr; }
        bool operator!=(const iterator& other) const { return ptr != other.ptr; }
        bool operator<(const iterator& other) const { return ptr < other.ptr; }
        operator T*() const { return ptr; }
    };

    class const_iterator : public std::iterator<std::random_access_iterator_tag, const T> {
    private:
        const T* ptr;
    public:
        const_iterator(const T* p = nullptr) : ptr(p) {}
        const_iterator(const iterator& it) : ptr(it.operator T*()) {}
        const_iterator& operator++() { ++ptr; return *this; }
        const_iterator operator++(int) { const_iterator tmp(*this); ++ptr; return tmp; }
        const_iterator& operator--() { --ptr; return *this; }
        const_iterator operator--(int) { const_iterator tmp(*this); --ptr; return tmp; }
        const T& operator*() const { return *ptr; }
        bool operator==(const const_iterator& other) const { return ptr == other.ptr; }
        bool operator!=(const const_iterator& other) const { return ptr != other.ptr; }
    };

    //конструкторы
    vector() : size(0), capacity(0), data(nullptr) {}

    vector(const vector& oth) : size(oth.size), capacity(oth.capacity) {
        if (capacity > 0) {
            data = new T[capacity];
            std::copy(oth.data, oth.data + size, data);
        } else {
            data = nullptr;
        }
    }

    vector(size_t length, const T& value) : size(length), capacity(length) {
        data = new T[capacity];
        for (size_t i = 0; i < size; i++) data[i] = value;
    }

    vector(size_t length, const T* array) : size(length), capacity(length) {
        data = new T[capacity];
        std::copy(array, array + size, data);
    }

    ~vector() {
        delete[] data;
    }
    vector& operator=(const vector& oth) {
        if (this != &oth) {
            if (capacity >= oth.size) {
                std::copy(oth.data, oth.data + oth.size, data);
                size = oth.size;
            } else {
                delete[] data;
                capacity = oth.size;
                data = new T[capacity];
                std::copy(oth.data, oth.data + oth.size, data);
                size = oth.size;
            }
        }
        return *this;
    }
    T operator[](size_t index) const { return data[index]; }
    T& operator[](size_t index) { return data[index]; }
    T at(size_t index) {
        if (index >= size) throw std::out_of_range("index out of range");
        return data[index];
    }

    T front() {
        if (size == 0) throw std::out_of_range("front out of range");
        return data[0];
    }

    T back() {
        if (size == 0) throw std::out_of_range("back out of range");
        return data[size - 1];
    }

    T* data_ptr() const { return data; }  // или просто data() как в STL

    iterator begin() { return iterator(data); }
    iterator end() { return iterator(data + size); }
    const_iterator begin() const { return const_iterator(data); }
    const_iterator end() const { return const_iterator(data + size); }
    const_iterator cbegin() const { return const_iterator(data); }
    const_iterator cend() const { return const_iterator(data + size); }
//вместимость
    bool empty() const { return size == 0; }
    size_t get_size() const { return size; }
    size_t get_capacity() const { return capacity; }
    void reserve(size_t new_capacity) {
        if (new_capacity <= capacity) return;
        T* new_data = new T[new_capacity];
        if (data) {
            std::copy(data, data + size, new_data);
            delete[] data;
        }
        data = new_data;
        capacity = new_capacity;
    }

    void clear() { size = 0; }

    void push_back(const T& value) {
        if (size >= capacity) {
            reserve(capacity == 0 ? 1 : capacity * 2);
        }
        data[size++] = value;
    }

    void pop_back() {
        if (size == 0) throw std::out_of_range("pop_back out of range");
        size--;
    }

    iterator insert(iterator position, const T& value) {
        size_t index = position - begin();
        if (size == capacity) {
            size_t new_cap = capacity == 0 ? 1 : capacity * 2;
            T* new_data = new T[new_cap];
            std::copy(data, data + index, new_data);
            new_data[index] = value;
            std::copy(data + index, data + size, new_data + index + 1);

            delete[] data;
            data = new_data;
            capacity = new_cap;
            size++;

            return iterator(data + index);
        } else {
            for (size_t i = size; i > index; --i) {
                data[i] = data[i - 1];
            }
            data[index] = value;
            size++;

            return iterator(data + index);
        }
    }

    void swap(vector& oth) {
        std::swap(data, oth.data);
        std::swap(size, oth.size);
        std::swap(capacity, oth.capacity);
    }

    friend std::ostream& operator<<(std::ostream& os, const vector& vec) {
        for (size_t i = 0; i < vec.size; i++) {
            os << vec.data[i];
            if (i < vec.size - 1) os << " ";
        }
        return os;
    }
};

int main() {
    vector<int> v;
    for (int i = 0; i < 10; i++) {
        v.push_back(i);
    }
    std::cout << "Vector after push_back: " << v << std::endl;
    std::sort(v.begin(), v.end(), std::greater<int>());
    std::cout << "After sort descending: " << v << std::endl;
    auto it = std::find_if(v.begin(), v.end(), [](int x) { return x > 5; });
    if (it != v.end()) {
        std::cout << "First element > 5: " << *it << std::endl;
    }
    v.insert(v.begin() + 3, 100);
    std::cout << "After insert at position 3: " << v << std::endl;
    return 0;
}