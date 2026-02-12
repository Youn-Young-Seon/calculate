#include "cpp/mylib/Calculator.hpp"

namespace mylib {

int Calculator::add(int a, int b) {
    return a + b;
}

int Calculator::divide(int a, int b) {
    if (b == 0) {
        throw std::invalid_argument("Division by zero");
    }
    return a / b;
}

double Calculator::average(const std::vector<int>& numbers) {
    if (numbers.empty()) {
        throw std::invalid_argument("Cannot calculate average of empty array");
    }

    long long sum = 0;
    for (int num : numbers) {
        sum += num;
    }

    return static_cast<double>(sum) / numbers.size();
}

} // namespace mylib
