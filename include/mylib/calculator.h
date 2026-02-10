#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <stddef.h>  /* size_t */

/**
 * @brief 두 정수를 더합니다
 * @param a 첫 번째 피연산자
 * @param b 두 번째 피연산자
 * @return 합계
 *
 * @note 오버플로우 체크는 호출자의 책임입니다
 */
int add(int a, int b);

/**
 * @brief 두 정수를 나눕니다
 * @param a 피제수
 * @param b 제수
 * @param result 결과를 저장할 포인터 (NULL 가능)
 * @return 성공 시 0, 실패 시 -1
 *
 * @warning b가 0이면 실패를 반환합니다
 */
int divide(int a, int b, int *result);

/**
 * @brief 배열의 평균을 계산합니다
 * @param arr 정수 배열
 * @param size 배열 크기
 * @return 평균값 (double)
 *
 * @pre arr != NULL && size > 0
 */
double average(const int *arr, size_t size);

#endif /* CALCULATOR_H */
