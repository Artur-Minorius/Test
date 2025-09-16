#pragma once
#include "List.h"
#include <stdexcept>


//And now we have a "stack"... With pre-built list it was easy
template<typename T>
class Stack {
private:
    List<T> list;

public:
    Stack() = default;

    void push(const T& value) {
        list.push_back(value);
    }

    void pop() {
        if (list.size() == 0)
            throw std::out_of_range("Stack is empty");
        list.pop_back();
    }

    T& top() {
        if (list.size() == 0)
            throw std::out_of_range("Stack is empty");
        return list[list.size() - 1];
    }

    const T& top() const {
        if (list.size() == 0)
            throw std::out_of_range("Stack is empty");
        return list[list.size() - 1];
    }

    bool empty() const {
        return list.size() == 0;
    }

    size_t size() const {
        return list.size();
    }

    void clear() {
        list.clear();
    }
};