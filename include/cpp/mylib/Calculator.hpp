#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

#include <vector>
#include <stdexcept>

namespace mylib {

/**
 * @brief 계산기 클래스
 *
 * 기본적인 산술 연산을 제공하는 클래스입니다.
 */
class Calculator {
public:
    /**
     * @brief 두 정수의 합을 계산합니다
     * @param a 첫 번째 정수
     * @param b 두 번째 정수
     * @return 두 수의 합
     */
    static int add(int a, int b);

    /**
     * @brief 두 정수의 나눗셈을 계산합니다
     * @param a 피제수
     * @param b 제수
     * @return 나눗셈 결과
     * @throws std::invalid_argument b가 0일 때
     */
    static int divide(int a, int b);

    /**
     * @brief 정수 배열의 평균을 계산합니다
     * @param numbers 정수 벡터
     * @return 평균값
     * @throws std::invalid_argument 배열이 비어있을 때
     */
    static double average(const std::vector<int>& numbers);
};

} // namespace mylib

#endif // CALCULATOR_HPP
