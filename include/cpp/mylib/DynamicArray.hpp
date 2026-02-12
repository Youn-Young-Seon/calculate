#ifndef DYNAMIC_ARRAY_HPP
#define DYNAMIC_ARRAY_HPP

#include <vector>
#include <stdexcept>
#include <cstddef>

namespace mylib {

/**
 * @brief 동적 배열 클래스
 *
 * std::vector를 래핑하여 자동으로 크기가 조절되는 정수 배열을 구현합니다.
 * RAII 패턴으로 자동 메모리 관리를 제공합니다.
 */
class DynamicArray {
private:
    std::vector<int> data_;

public:
    /**
     * @brief 기본 생성자 - 빈 배열을 생성합니다
     */
    DynamicArray() = default;

    /**
     * @brief 초기 용량을 지정하는 생성자
     * @param initial_capacity 초기 용량
     */
    explicit DynamicArray(size_t initial_capacity);

    /**
     * @brief 배열의 끝에 새로운 요소를 추가합니다
     * @param value 추가할 값
     */
    void push(int value);

    /**
     * @brief 특정 인덱스의 값을 가져옵니다
     * @param index 인덱스
     * @return 해당 인덱스의 값
     * @throws std::out_of_range 인덱스가 범위를 벗어날 때
     */
    int get(size_t index) const;

    /**
     * @brief 특정 인덱스에 값을 설정합니다
     * @param index 인덱스
     * @param value 설정할 값
     * @throws std::out_of_range 인덱스가 범위를 벗어날 때
     */
    void set(size_t index, int value);

    /**
     * @brief 배열의 마지막 요소를 제거하고 반환합니다
     * @return 제거된 값
     * @throws std::runtime_error 배열이 비어있을 때
     */
    int pop();

    /**
     * @brief 배열의 현재 크기를 반환합니다
     * @return 배열의 크기
     */
    size_t size() const;

    /**
     * @brief 배열의 현재 용량을 반환합니다
     * @return 배열의 용량
     */
    size_t capacity() const;

    /**
     * @brief 배열이 비어있는지 확인합니다
     * @return 비어있으면 true, 아니면 false
     */
    bool isEmpty() const;

    /**
     * @brief 배열의 모든 요소를 제거합니다
     */
    void clear();

    /**
     * @brief 배열에 [] 연산자로 접근 (읽기 전용)
     * @param index 인덱스
     * @return 해당 인덱스의 값
     * @throws std::out_of_range 인덱스가 범위를 벗어날 때
     */
    int operator[](size_t index) const;

    /**
     * @brief 배열에 [] 연산자로 접근 (쓰기 가능)
     * @param index 인덱스
     * @return 해당 인덱스의 참조
     * @throws std::out_of_range 인덱스가 범위를 벗어날 때
     */
    int& operator[](size_t index);
};

} // namespace mylib

#endif // DYNAMIC_ARRAY_HPP
