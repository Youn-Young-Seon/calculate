#include "cpp/mylib/CSVParser.hpp"
#include <iostream>
#include <cassert>
#include <stdexcept>

using namespace mylib;

void test_parse_and_get() {
    CSVParser parser;
    parser.parse("tests/test_data.csv");

    assert(parser.rowCount() > 0);
    assert(parser.colCount() > 0);

    std::cout << "✓ test_parse_and_get passed\n";
}

void test_find_column() {
    CSVParser parser;
    parser.parse("tests/test_data.csv");

    // test_data.csv 파일에 따라 헤더가 다를 수 있으므로
    // 첫 번째 헤더로 테스트
    const auto& headers = parser.headers();
    if (!headers.empty()) {
        size_t col = parser.findColumn(headers[0]);
        assert(col == 0);
    }

    std::cout << "✓ test_find_column passed\n";
}

void test_get_row() {
    CSVParser parser;
    parser.parse("tests/test_data.csv");

    if (parser.rowCount() > 0) {
        auto row = parser.getRow(0);
        assert(row.size() == parser.colCount());
    }

    std::cout << "✓ test_get_row passed\n";
}

void test_write() {
    CSVParser parser;
    parser.parse("tests/test_data.csv");

    // 임시 파일로 쓰기
    parser.write("tests/test_output.csv");

    // 다시 읽어서 비교
    CSVParser parser2;
    parser2.parse("tests/test_output.csv");

    assert(parser.rowCount() == parser2.rowCount());
    assert(parser.colCount() == parser2.colCount());

    std::cout << "✓ test_write passed\n";
}

void test_invalid_file() {
    CSVParser parser;

    try {
        parser.parse("nonexistent_file.csv");
        assert(false && "Should have thrown exception");
    } catch (const std::runtime_error& e) {
        // 예외 발생 - 정상
    }

    std::cout << "✓ test_invalid_file passed\n";
}

void test_out_of_range() {
    CSVParser parser;
    parser.parse("tests/test_data.csv");

    try {
        parser.get(9999, 0);
        assert(false && "Should have thrown exception");
    } catch (const std::out_of_range& e) {
        // 예외 발생 - 정상
    }

    std::cout << "✓ test_out_of_range passed\n";
}

int main() {
    std::cout << "Running CSVParser C++ tests...\n\n";

    test_parse_and_get();
    test_find_column();
    test_get_row();
    test_write();
    test_invalid_file();
    test_out_of_range();

    std::cout << "\n✓ All CSVParser tests passed!\n";
    return 0;
}
