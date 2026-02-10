#include "mylib/calculator.h"
#include <stddef.h>  /* size_t */

/* 왜 이렇게?: 간단한 함수는 인라인으로 최적화 가능성 */
int add(int a, int b) {
    return a + b;
}

/*
 * 왜 이렇게?:
 * - 0으로 나누기 방지 (정의되지 않은 동작 예방)
 * - result가 NULL일 수 있으므로 체크 후 사용
 * - 반환값으로 성공/실패 전달 (POSIX 스타일)
 */
int divide(int a, int b, int *result) {
    if (b == 0) {
        return -1;  /* 실패 */
    }

    if (result != NULL) {
        *result = a / b;
    }

    return 0;  /* 성공 */
}

/*
 * 왜 이렇게?:
 * - const int *arr: 배열 수정 방지 (side effect 없음)
 * - size_t: 음수 크기는 의미 없으므로 unsigned 사용
 * - double 반환: 정수 나눗셈 손실 방지
 * - 방어적 프로그래밍: NULL 체크, 0 크기 체크
 */
double average(const int *arr, size_t size) {
    if (arr == NULL || size == 0) {
        return 0.0;
    }

    long long sum = 0;  /* 오버플로우 방지용 큰 타입 */

    for (size_t i = 0; i < size; i++) {
        sum += arr[i];
    }

    return (double)sum / size;  /* 명시적 캐스팅 */
}
