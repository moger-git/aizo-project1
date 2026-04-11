#ifndef AIZO_PROJECT1_DYNAMICARRAY_H
#define AIZO_PROJECT1_DYNAMICARRAY_H

#include <cstddef>
#include <iostream>

template<typename T>
class DynamicArray {
    T* data_;
    std::size_t size_;
    std::size_t capacity_;

    void reserve(std::size_t new_capacity);

public:
    DynamicArray() : data_(nullptr), size_(0), capacity_(0) {}

    ~DynamicArray() {
        delete[] data_;
    }

    void push_back(const T& value) {
        if (size_ == capacity_) {
            const std::size_t new_capacity = (capacity_ == 0) ? 4 : capacity_ * 2;
            reserve(new_capacity);
        }

        data_[size_] = value;
        ++size_;
    }

    void clear() {
        delete[] data_;
        data_ = nullptr;
        size_ = 0;
        capacity_ = 0;
    }

    [[nodiscard]] std::size_t size() const {
        return size_;
    }

    T& at(std::size_t index) {
        return data_[index];
    }

    const T& at(std::size_t index) const {
        return data_[index];
    }

    void print() const {
        for (std::size_t i = 0; i < size_; ++i) {
            std::cout << data_[i] << " ";
        }
        std::cout << "\n";
    }
};

template <typename T>
void DynamicArray<T>::reserve(std::size_t new_capacity)
{
    if (new_capacity <= capacity_) {
        return;
    }

    T* new_data = new T[new_capacity];
    for (std::size_t i = 0; i < size_; ++i) {
        new_data[i] = data_[i];
    }

    delete[] data_;
    data_ = new_data;
    capacity_ = new_capacity;
}


#endif //AIZO_PROJECT1_DYNAMICARRAY_H
