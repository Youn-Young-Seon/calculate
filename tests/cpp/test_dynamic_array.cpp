#include "cpp/mylib/DynamicArray.hpp"
#include <iostream>
#include <cassert>
#include <stdexcept>

using namespace mylib;

void test_push_and_get() {
    DynamicArray arr;
    arr.push(10);
    arr.push(20);
    arr.push(30);

    assert(arr.get(0) == 10);
    assert(arr.get(1) == 20);
    assert(arr.get(2) == 30);
    assert(arr.size() == 3);

    std::cout << "✓ test_push_and_get passed\n";
}

void test_set() {
    DynamicArray arr;
    arr.push(1);
    arr.push(2);
    arr.push(3);

    arr.set(1, 99);
    assert(arr.get(1) == 99);

    std::cout << "✓ test_set passed\n";
}

void test_pop() {
    DynamicArray arr;
    arr.push(10);
    arr.push(20);
    arr.push(30);

    assert(arr.pop() == 30);
    assert(arr.size() == 2);
    assert(arr.pop() == 20);
    assert(arr.size() == 1);

    std::cout << "✓ test_pop passed\n";
}

void test_operator_brackets() {
    DynamicArray arr;
    arr.push(100);
    arr.push(200);
    arr.push(300);

    assert(arr[0] == 100);
    assert(arr[1] == 200);
    assert(arr[2] == 300);

    arr[1] = 999;
    assert(arr[1] == 999);

    std::cout << "✓ test_operator_brackets passed\n";
}

void test_isEmpty_and_clear() {
    DynamicArray arr;
    assert(arr.isEmpty() == true);

    arr.push(1);
    assert(arr.isEmpty() == false);

    arr.clear();
    assert(arr.isEmpty() == true);
    assert(arr.size() == 0);

    std::cout << "✓ test_isEmpty_and_clear passed\n";
}

void test_capacity() {
    DynamicArray arr(5);
    assert(arr.capacity() >= 5);
    assert(arr.size() == 0);

    for (int i = 0; i < 10; ++i) {
        arr.push(i);
    }
    assert(arr.size() == 10);
    assert(arr.capacity() >= 10);

    std::cout << "✓ test_capacity passed\n";
}

void test_out_of_range() {
    DynamicArray arr;
    arr.push(1);

    try {
        arr.get(10);
        assert(false && "Should have thrown exception");
    } catch (const std::out_of_range& e) {
        // 예외 발생 - 정상
    }

    std::cout << "✓ test_out_of_range passed\n";
}

void test_pop_empty() {
    DynamicArray arr;

    try {
        arr.pop();
        assert(false && "Should have thrown exception");
    } catch (const std::runtime_error& e) {
        // 예외 발생 - 정상
    }

    std::cout << "✓ test_pop_empty passed\n";
}

int main() {
    std::cout << "Running DynamicArray C++ tests...\n\n";

    test_push_and_get();
    test_set();
    test_pop();
    test_operator_brackets();
    test_isEmpty_and_clear();
    test_capacity();
    test_out_of_range();
    test_pop_empty();

    std::cout << "\n✓ All DynamicArray tests passed!\n";
    return 0;
}
