#pragma once
#include <cstddef>
#include <stdexcept>

template<typename T>
class List {
public:
    List();
    List(int size) { data = new T[size]; capacity = size; };
    ~List();

    void push_back(const T& value);
    void pop_back();
    T& operator[](size_t index);
    const T& operator[](size_t index) const;
    void remove(size_t index);
    size_t size() const;
    bool empty();
    void clear();
    template<typename Func>
    void ForEach(Func func) {
        for (size_t i = 0; i < _size; ++i) {
            func(data[i]);
        }
    }
    bool contains(T& value);
    template <typename M>
    M* ToArray();

    List<T>* Copy();
private:
    T* data;
    size_t _size;
    size_t capacity;

    void resize(size_t newCap);
};

#include "List.tpp"

