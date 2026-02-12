#include "cpp/mylib/CSVParser.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

namespace mylib {

void CSVParser::parse(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    headers_.clear();
    rows_.clear();

    std::string line;
    bool is_header = true;

    while (std::getline(file, line)) {
        // 빈 줄 건너뛰기
        if (line.empty()) {
            continue;
        }

        std::vector<std::string> row;
        std::stringstream ss(line);
        std::string cell;

        while (std::getline(ss, cell, ',')) {
            // 앞뒤 공백 제거
            cell.erase(0, cell.find_first_not_of(" \t\r\n"));
            cell.erase(cell.find_last_not_of(" \t\r\n") + 1);
            row.push_back(cell);
        }

        if (is_header) {
            headers_ = row;
            is_header = false;
        } else {
            rows_.push_back(row);
        }
    }

    file.close();
}

std::string CSVParser::get(size_t row, size_t col) const {
    if (row >= rows_.size()) {
        throw std::out_of_range("Row index out of range");
    }
    if (col >= rows_[row].size()) {
        throw std::out_of_range("Column index out of range");
    }
    return rows_[row][col];
}

size_t CSVParser::findColumn(const std::string& header_name) const {
    auto it = std::find(headers_.begin(), headers_.end(), header_name);
    if (it == headers_.end()) {
        throw std::runtime_error("Header not found: " + header_name);
    }
    return std::distance(headers_.begin(), it);
}

std::vector<std::string> CSVParser::getRow(size_t row) const {
    if (row >= rows_.size()) {
        throw std::out_of_range("Row index out of range");
    }
    return rows_[row];
}

void CSVParser::print() const {
    // 헤더 출력
    std::cout << "Headers: ";
    for (size_t i = 0; i < headers_.size(); ++i) {
        std::cout << headers_[i];
        if (i < headers_.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "\n\n";

    // 데이터 출력
    for (size_t i = 0; i < rows_.size(); ++i) {
        std::cout << "Row " << i << ": ";
        for (size_t j = 0; j < rows_[i].size(); ++j) {
            std::cout << rows_[i][j];
            if (j < rows_[i].size() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << "\n";
    }
}

void CSVParser::write(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file for writing: " + filename);
    }

    // 헤더 쓰기
    for (size_t i = 0; i < headers_.size(); ++i) {
        file << headers_[i];
        if (i < headers_.size() - 1) {
            file << ",";
        }
    }
    file << "\n";

    // 데이터 쓰기
    for (const auto& row : rows_) {
        for (size_t i = 0; i < row.size(); ++i) {
            file << row[i];
            if (i < row.size() - 1) {
                file << ",";
            }
        }
        file << "\n";
    }

    file.close();
}

size_t CSVParser::rowCount() const {
    return rows_.size();
}

size_t CSVParser::colCount() const {
    return headers_.size();
}

const std::vector<std::string>& CSVParser::headers() const {
    return headers_;
}

} // namespace mylib
