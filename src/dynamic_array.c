#include "mylib/dynamic_array.h"
#include <stdlib.h>  /* malloc, realloc, free */
#include <string.h>  /* memcpy */

/* 기본 초기 용량 */
#define DEFAULT_CAPACITY 8

/* 용량 증가 배수 */
#define GROWTH_FACTOR 2

/**
 * @brief 내부 함수: 배열의 용량을 확장합니다
 * @param arr 동적 배열 포인터
 * @param new_capacity 새로운 용량
 * @return 성공 시 0, 실패 시 -1
 */
static int da_resize(DynamicArray *arr, size_t new_capacity) {
    if (arr == NULL || new_capacity == 0) {
        return -1;
    }

    /* realloc으로 메모리 재할당 */
    int *new_data = (int *)realloc(arr->data, new_capacity * sizeof(int));
    if (new_data == NULL) {
        /* 메모리 할당 실패 - 기존 데이터는 유지됨 */
        return -1;
    }

    arr->data = new_data;
    arr->capacity = new_capacity;

    return 0;
}

DynamicArray* da_create(size_t initial_capacity) {
    /* 동적 배열 구조체 할당 */
    DynamicArray *arr = (DynamicArray *)malloc(sizeof(DynamicArray));
    if (arr == NULL) {
        return NULL;
    }

    /* 초기 용량 설정 */
    if (initial_capacity == 0) {
        initial_capacity = DEFAULT_CAPACITY;
    }

    /* 데이터 배열 할당 */
    arr->data = (int *)malloc(initial_capacity * sizeof(int));
    if (arr->data == NULL) {
        free(arr);  /* 구조체 메모리 해제 */
        return NULL;
    }

    arr->size = 0;
    arr->capacity = initial_capacity;

    return arr;
}

int da_push(DynamicArray *arr, int value) {
    if (arr == NULL) {
        return -1;
    }

    /* 용량이 부족하면 확장 */
    if (arr->size >= arr->capacity) {
        size_t new_capacity = arr->capacity * GROWTH_FACTOR;
        if (da_resize(arr, new_capacity) != 0) {
            return -1;  /* 확장 실패 */
        }
    }

    /* 새 요소 추가 */
    arr->data[arr->size] = value;
    arr->size++;

    return 0;
}

int da_get(const DynamicArray *arr, size_t index, int *value) {
    /* NULL 체크 */
    if (arr == NULL || value == NULL) {
        return -1;
    }

    /* 범위 체크 */
    if (index >= arr->size) {
        return -1;
    }

    *value = arr->data[index];
    return 0;
}

int da_set(DynamicArray *arr, size_t index, int value) {
    /* NULL 체크 */
    if (arr == NULL) {
        return -1;
    }

    /* 범위 체크 */
    if (index >= arr->size) {
        return -1;
    }

    arr->data[index] = value;
    return 0;
}

int da_pop(DynamicArray *arr, int *value) {
    /* NULL 체크 */
    if (arr == NULL) {
        return -1;
    }

    /* 빈 배열 체크 */
    if (arr->size == 0) {
        return -1;
    }

    /* 마지막 요소 제거 */
    arr->size--;

    /* 값 반환 (선택적) */
    if (value != NULL) {
        *value = arr->data[arr->size];
    }

    return 0;
}

size_t da_size(const DynamicArray *arr) {
    if (arr == NULL) {
        return 0;
    }
    return arr->size;
}

size_t da_capacity(const DynamicArray *arr) {
    if (arr == NULL) {
        return 0;
    }
    return arr->capacity;
}

int da_is_empty(const DynamicArray *arr) {
    if (arr == NULL) {
        return 1;
    }
    return arr->size == 0;
}

void da_clear(DynamicArray *arr) {
    if (arr == NULL) {
        return;
    }
    arr->size = 0;
}

void da_destroy(DynamicArray *arr) {
    if (arr == NULL) {
        return;
    }

    /* 데이터 배열 해제 */
    free(arr->data);

    /* 구조체 해제 */
    free(arr);
}
