#include "List.h"

template<typename T>
List<T>::List() : data(nullptr), _size(0), capacity(0) {}

template<typename T>
List<T>::~List() {
    delete[] data;
}

template<typename T>
void List<T>::push_back(const T& value) {
    if (_size == capacity) {
        resize(capacity == 0 ? 1 : capacity + 1);
    }
    data[_size++] = value;
}

template<typename T>
void List<T>::pop_back() {
    if (_size == 0) throw std::out_of_range("List is empty");
    --_size;
}

template<typename T>
T& List<T>::operator[](size_t index) {
    if (index >= _size) throw std::out_of_range("Index out of range");
    return data[index];
}

template<typename T>
const T& List<T>::operator[](size_t index) const {
    if (index >= _size) throw std::out_of_range("Index out of range");
    return data[index];
}

template<typename T>
void List<T>::remove(size_t index) {
    if (index >= _size) throw std::out_of_range("Index out of range");
    for (size_t i = index; i < _size - 1; ++i)
        data[i] = data[i + 1];
    --_size;
}

template<typename T>
size_t List<T>::size() const {
    return _size;
}

template<typename T>
void List<T>::clear() {
    _size = 0;
}

template<typename T>
void List<T>::resize(size_t newCap) {
    T* newData = new T[newCap];
    for (size_t i = 0; i < _size; ++i)
        newData[i] = data[i];
    delete[] data;
    data = newData;
    capacity = newCap;
}

template<typename T>
bool List<T>::contains(T& value) {
    for (size_t i = 0; i < _size; ++i) {
        if (value == data[i])
            return true;
    }
    return false;
}

template <typename T>
template <typename M>
M* List<T>::ToArray() {
    M* array = new M[_size];
    for (int i = 0; i < _size; i++) {
        array[i] = static_cast<M>(data[i]);
    }
    return array;
}

template <typename T>
List<T>* List<T>::Copy() {
        List<T>* newList = new List<T>();   
        ForEach([&](T elem) {newList->push_back(elem); });
        return newList;
}

template <typename T>
bool List<T>::empty() {
        return _size == 0;
};