#ifndef CSV_PARSER_HPP
#define CSV_PARSER_HPP

#include <string>
#include <vector>
#include <stdexcept>
#include <cstddef>

namespace mylib {

/**
 * @brief CSV 파서 클래스
 *
 * CSV 파일을 읽고 쓰는 기능을 제공합니다.
 * std::string과 std::vector를 사용하여 자동 메모리 관리를 제공합니다.
 */
class CSVParser {
private:
    std::vector<std::string> headers_;
    std::vector<std::vector<std::string>> rows_;

public:
    /**
     * @brief 기본 생성자
     */
    CSVParser() = default;

    /**
     * @brief CSV 파일을 파싱합니다
     * @param filename 읽을 CSV 파일 경로
     * @throws std::runtime_error 파일을 열 수 없을 때
     *
     * @note 첫 번째 행은 헤더로 처리됩니다
     */
    void parse(const std::string& filename);

    /**
     * @brief 특정 셀의 값을 가져옵니다
     * @param row 행 인덱스 (0부터 시작)
     * @param col 열 인덱스 (0부터 시작)
     * @return 셀의 문자열 값
     * @throws std::out_of_range 인덱스가 범위를 벗어날 때
     */
    std::string get(size_t row, size_t col) const;

    /**
     * @brief 헤더 이름으로 열 인덱스를 찾습니다
     * @param header_name 찾을 헤더 이름
     * @return 열 인덱스
     * @throws std::runtime_error 헤더를 찾지 못했을 때
     */
    size_t findColumn(const std::string& header_name) const;

    /**
     * @brief 특정 행의 모든 값을 가져옵니다
     * @param row 행 인덱스
     * @return 행의 문자열 벡터
     * @throws std::out_of_range 인덱스가 범위를 벗어날 때
     */
    std::vector<std::string> getRow(size_t row) const;

    /**
     * @brief CSV 데이터를 콘솔에 출력합니다
     *
     * @note 디버깅 및 확인용
     */
    void print() const;

    /**
     * @brief CSV 데이터를 파일로 저장합니다
     * @param filename 저장할 파일 경로
     * @throws std::runtime_error 파일을 열 수 없을 때
     */
    void write(const std::string& filename) const;

    /**
     * @brief CSV 데이터의 행 개수를 반환합니다
     * @return 행 개수 (헤더 제외)
     */
    size_t rowCount() const;

    /**
     * @brief CSV 데이터의 열 개수를 반환합니다
     * @return 열 개수
     */
    size_t colCount() const;

    /**
     * @brief 헤더 목록을 반환합니다
     * @return 헤더 벡터
     */
    const std::vector<std::string>& headers() const;
};

} // namespace mylib

#endif // CSV_PARSER_HPP
