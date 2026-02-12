#include "cpp/mylib/DynamicArray.hpp"

namespace mylib {

DynamicArray::DynamicArray(size_t initial_capacity) {
    data_.reserve(initial_capacity);
}

void DynamicArray::push(int value) {
    data_.push_back(value);
}

int DynamicArray::get(size_t index) const {
    if (index >= data_.size()) {
        throw std::out_of_range("Index out of range");
    }
    return data_[index];
}

void DynamicArray::set(size_t index, int value) {
    if (index >= data_.size()) {
        throw std::out_of_range("Index out of range");
    }
    data_[index] = value;
}

int DynamicArray::pop() {
    if (data_.empty()) {
        throw std::runtime_error("Cannot pop from empty array");
    }
    int value = data_.back();
    data_.pop_back();
    return value;
}

size_t DynamicArray::size() const {
    return data_.size();
}

size_t DynamicArray::capacity() const {
    return data_.capacity();
}

bool DynamicArray::isEmpty() const {
    return data_.empty();
}

void DynamicArray::clear() {
    data_.clear();
}

int DynamicArray::operator[](size_t index) const {
    if (index >= data_.size()) {
        throw std::out_of_range("Index out of range");
    }
    return data_[index];
}

int& DynamicArray::operator[](size_t index) {
    if (index >= data_.size()) {
        throw std::out_of_range("Index out of range");
    }
    return data_[index];
}

} // namespace mylib
