#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stddef.h>  /* size_t */

/**
 * @brief 동적 배열 구조체
 *
 * 자동으로 크기가 조절되는 정수 배열을 구현합니다.
 * 내부적으로 malloc/realloc을 사용하여 메모리를 관리합니다.
 */
typedef struct {
    int *data;           /**< 실제 데이터를 저장하는 포인터 */
    size_t size;         /**< 현재 저장된 요소의 개수 */
    size_t capacity;     /**< 할당된 메모리의 최대 용량 */
} DynamicArray;

/**
 * @brief 새로운 동적 배열을 생성합니다
 * @param initial_capacity 초기 용량 (0이면 기본값 사용)
 * @return 생성된 동적 배열 포인터, 실패 시 NULL
 *
 * @note 사용 후 반드시 da_destroy()로 메모리를 해제해야 합니다
 *
 * @example
 * DynamicArray *arr = da_create(10);
 * if (arr != NULL) {
 *     // 사용...
 *     da_destroy(arr);
 * }
 */
DynamicArray* da_create(size_t initial_capacity);

/**
 * @brief 배열의 끝에 새로운 요소를 추가합니다
 * @param arr 동적 배열 포인터
 * @param value 추가할 값
 * @return 성공 시 0, 실패 시 -1
 *
 * @note 용량이 부족하면 자동으로 2배 확장됩니다
 */
int da_push(DynamicArray *arr, int value);

/**
 * @brief 특정 인덱스의 값을 가져옵니다
 * @param arr 동적 배열 포인터
 * @param index 가져올 인덱스
 * @param value 결과를 저장할 포인터
 * @return 성공 시 0, 실패 시 -1 (범위 초과 또는 NULL 포인터)
 */
int da_get(const DynamicArray *arr, size_t index, int *value);

/**
 * @brief 특정 인덱스에 값을 설정합니다
 * @param arr 동적 배열 포인터
 * @param index 설정할 인덱스
 * @param value 설정할 값
 * @return 성공 시 0, 실패 시 -1 (범위 초과 또는 NULL 포인터)
 */
int da_set(DynamicArray *arr, size_t index, int value);

/**
 * @brief 배열의 마지막 요소를 제거합니다
 * @param arr 동적 배열 포인터
 * @param value 제거된 값을 저장할 포인터 (NULL 가능)
 * @return 성공 시 0, 실패 시 -1 (빈 배열 또는 NULL 포인터)
 */
int da_pop(DynamicArray *arr, int *value);

/**
 * @brief 배열의 현재 크기를 반환합니다
 * @param arr 동적 배열 포인터
 * @return 배열의 크기, arr이 NULL이면 0
 */
size_t da_size(const DynamicArray *arr);

/**
 * @brief 배열의 현재 용량을 반환합니다
 * @param arr 동적 배열 포인터
 * @return 배열의 용량, arr이 NULL이면 0
 */
size_t da_capacity(const DynamicArray *arr);

/**
 * @brief 배열이 비어있는지 확인합니다
 * @param arr 동적 배열 포인터
 * @return 비어있으면 1, 아니면 0
 */
int da_is_empty(const DynamicArray *arr);

/**
 * @brief 배열의 모든 요소를 제거합니다 (메모리는 유지)
 * @param arr 동적 배열 포인터
 */
void da_clear(DynamicArray *arr);

/**
 * @brief 동적 배열을 파괴하고 메모리를 해제합니다
 * @param arr 동적 배열 포인터
 *
 * @warning 이 함수 호출 후 arr 포인터를 사용하면 안 됩니다
 */
void da_destroy(DynamicArray *arr);

#endif /* DYNAMIC_ARRAY_H */
