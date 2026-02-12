#include "cpp/mylib/Calculator.hpp"
#include "cpp/mylib/DynamicArray.hpp"
#include "cpp/mylib/CSVParser.hpp"
#include <iostream>
#include <vector>

using namespace mylib;

int main() {
    std::cout << "=== C++ Library Demo ===\n\n";

    // Calculator 데모
    std::cout << "1. Calculator Demo:\n";
    std::cout << "   5 + 3 = " << Calculator::add(5, 3) << "\n";
    std::cout << "   10 / 2 = " << Calculator::divide(10, 2) << "\n";

    std::vector<int> nums = {1, 2, 3, 4, 5};
    std::cout << "   Average of {1,2,3,4,5} = " << Calculator::average(nums) << "\n\n";

    // DynamicArray 데모
    std::cout << "2. DynamicArray Demo:\n";
    DynamicArray arr;
    arr.push(10);
    arr.push(20);
    arr.push(30);
    std::cout << "   Array: [" << arr[0] << ", " << arr[1] << ", " << arr[2] << "]\n";
    std::cout << "   Size: " << arr.size() << ", Capacity: " << arr.capacity() << "\n\n";

    // CSVParser 데모
    std::cout << "3. CSVParser Demo:\n";
    try {
        CSVParser parser;
        parser.parse("tests/test_data.csv");
        std::cout << "   CSV loaded: " << parser.rowCount() << " rows, "
                  << parser.colCount() << " columns\n";
        parser.print();
    } catch (const std::exception& e) {
        std::cout << "   CSV file not found (this is okay for demo)\n";
    }

    std::cout << "\n=== Demo Complete ===\n";
    return 0;
}
