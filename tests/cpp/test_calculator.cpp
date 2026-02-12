#include "cpp/mylib/Calculator.hpp"
#include <iostream>
#include <cassert>
#include <stdexcept>

using namespace mylib;

void test_add() {
    assert(Calculator::add(2, 3) == 5);
    assert(Calculator::add(-1, 1) == 0);
    assert(Calculator::add(0, 0) == 0);
    std::cout << "✓ test_add passed\n";
}

void test_divide() {
    assert(Calculator::divide(10, 2) == 5);
    assert(Calculator::divide(7, 3) == 2);
    assert(Calculator::divide(-10, 2) == -5);

    // 0으로 나누기 테스트
    try {
        Calculator::divide(10, 0);
        assert(false && "Should have thrown exception");
    } catch (const std::invalid_argument& e) {
        // 예외 발생 - 정상
    }

    std::cout << "✓ test_divide passed\n";
}

void test_average() {
    std::vector<int> nums1 = {1, 2, 3, 4, 5};
    assert(Calculator::average(nums1) == 3.0);

    std::vector<int> nums2 = {10, 20};
    assert(Calculator::average(nums2) == 15.0);

    std::vector<int> nums3 = {100};
    assert(Calculator::average(nums3) == 100.0);

    // 빈 배열 테스트
    try {
        std::vector<int> empty;
        Calculator::average(empty);
        assert(false && "Should have thrown exception");
    } catch (const std::invalid_argument& e) {
        // 예외 발생 - 정상
    }

    std::cout << "✓ test_average passed\n";
}

int main() {
    std::cout << "Running Calculator C++ tests...\n\n";

    test_add();
    test_divide();
    test_average();

    std::cout << "\n✓ All Calculator tests passed!\n";
    return 0;
}
