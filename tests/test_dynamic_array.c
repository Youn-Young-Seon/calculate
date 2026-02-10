#include <stdio.h>
#include <assert.h>
#include "mylib/dynamic_array.h"

/* 간단한 테스트 프레임워크 */
static int tests_run = 0;
static int tests_passed = 0;

#define TEST_ASSERT(condition, message) \
    do { \
        tests_run++; \
        if (condition) { \
            printf("PASS: %s\n", message); \
            tests_passed++; \
        } else { \
            printf("FAIL: %s\n", message); \
        } \
    } while(0)

#define TEST_ASSERT_EQUAL_INT(expected, actual, message) \
    do { \
        tests_run++; \
        if ((expected) == (actual)) { \
            printf("PASS: %s\n", message); \
            tests_passed++; \
        } else { \
            printf("FAIL: %s (expected %d, got %d)\n", message, expected, actual); \
        } \
    } while(0)

#define TEST_ASSERT_EQUAL_SIZE(expected, actual, message) \
    do { \
        tests_run++; \
        if ((expected) == (actual)) { \
            printf("PASS: %s\n", message); \
            tests_passed++; \
        } else { \
            printf("FAIL: %s (expected %zu, got %zu)\n", message, (size_t)expected, (size_t)actual); \
        } \
    } while(0)

/* 생성 및 파괴 테스트 */
void test_create_and_destroy(void) {
    DynamicArray *arr = da_create(0);
    TEST_ASSERT(arr != NULL, "da_create should return non-NULL");

    if (arr != NULL) {
        TEST_ASSERT_EQUAL_SIZE(0, da_size(arr), "New array size should be 0");
        TEST_ASSERT_EQUAL_SIZE(8, da_capacity(arr), "Default capacity should be 8");
        TEST_ASSERT(da_is_empty(arr), "New array should be empty");
        da_destroy(arr);
    }
}

void test_create_with_capacity(void) {
    DynamicArray *arr = da_create(20);
    TEST_ASSERT(arr != NULL, "da_create(20) should return non-NULL");

    if (arr != NULL) {
        TEST_ASSERT_EQUAL_SIZE(0, da_size(arr), "New array size should be 0");
        TEST_ASSERT_EQUAL_SIZE(20, da_capacity(arr), "Initial capacity should be 20");
        da_destroy(arr);
    }
}

/* Push 테스트 */
void test_push_single_element(void) {
    DynamicArray *arr = da_create(0);
    if (arr == NULL) {
        printf("SKIP: test_push_single_element (allocation failed)\n");
        return;
    }

    int status = da_push(arr, 42);
    TEST_ASSERT_EQUAL_INT(0, status, "da_push should return 0 on success");
    TEST_ASSERT_EQUAL_SIZE(1, da_size(arr), "Size should be 1 after push");
    TEST_ASSERT(!da_is_empty(arr), "Array should not be empty after push");

    int value;
    da_get(arr, 0, &value);
    TEST_ASSERT_EQUAL_INT(42, value, "First element should be 42");

    da_destroy(arr);
}

void test_push_multiple_elements(void) {
    DynamicArray *arr = da_create(0);
    if (arr == NULL) {
        printf("SKIP: test_push_multiple_elements (allocation failed)\n");
        return;
    }

    /* 여러 요소 추가 */
    for (int i = 0; i < 10; i++) {
        da_push(arr, i * 10);
    }

    TEST_ASSERT_EQUAL_SIZE(10, da_size(arr), "Size should be 10 after 10 pushes");

    /* 값 확인 */
    int value;
    da_get(arr, 0, &value);
    TEST_ASSERT_EQUAL_INT(0, value, "First element should be 0");

    da_get(arr, 9, &value);
    TEST_ASSERT_EQUAL_INT(90, value, "Last element should be 90");

    da_destroy(arr);
}

void test_push_triggers_resize(void) {
    DynamicArray *arr = da_create(2);  /* 작은 초기 용량 */
    if (arr == NULL) {
        printf("SKIP: test_push_triggers_resize (allocation failed)\n");
        return;
    }

    TEST_ASSERT_EQUAL_SIZE(2, da_capacity(arr), "Initial capacity should be 2");

    /* 용량 초과로 push */
    da_push(arr, 1);
    da_push(arr, 2);
    da_push(arr, 3);  /* 여기서 확장 발생 */

    TEST_ASSERT_EQUAL_SIZE(3, da_size(arr), "Size should be 3");
    TEST_ASSERT(da_capacity(arr) >= 3, "Capacity should be expanded");

    da_destroy(arr);
}

/* Get/Set 테스트 */
void test_get_and_set(void) {
    DynamicArray *arr = da_create(0);
    if (arr == NULL) {
        printf("SKIP: test_get_and_set (allocation failed)\n");
        return;
    }

    da_push(arr, 100);
    da_push(arr, 200);
    da_push(arr, 300);

    /* Set 테스트 */
    int status = da_set(arr, 1, 999);
    TEST_ASSERT_EQUAL_INT(0, status, "da_set should return 0 on success");

    /* Get 테스트 */
    int value;
    status = da_get(arr, 1, &value);
    TEST_ASSERT_EQUAL_INT(0, status, "da_get should return 0 on success");
    TEST_ASSERT_EQUAL_INT(999, value, "Element at index 1 should be 999");

    da_destroy(arr);
}

void test_get_out_of_bounds(void) {
    DynamicArray *arr = da_create(0);
    if (arr == NULL) {
        printf("SKIP: test_get_out_of_bounds (allocation failed)\n");
        return;
    }

    da_push(arr, 10);

    int value;
    int status = da_get(arr, 10, &value);  /* 범위 초과 */
    TEST_ASSERT_EQUAL_INT(-1, status, "da_get out of bounds should return -1");

    da_destroy(arr);
}

/* Pop 테스트 */
void test_pop(void) {
    DynamicArray *arr = da_create(0);
    if (arr == NULL) {
        printf("SKIP: test_pop (allocation failed)\n");
        return;
    }

    da_push(arr, 10);
    da_push(arr, 20);
    da_push(arr, 30);

    int value;
    int status = da_pop(arr, &value);
    TEST_ASSERT_EQUAL_INT(0, status, "da_pop should return 0 on success");
    TEST_ASSERT_EQUAL_INT(30, value, "Popped value should be 30");
    TEST_ASSERT_EQUAL_SIZE(2, da_size(arr), "Size should be 2 after pop");

    da_destroy(arr);
}

void test_pop_empty_array(void) {
    DynamicArray *arr = da_create(0);
    if (arr == NULL) {
        printf("SKIP: test_pop_empty_array (allocation failed)\n");
        return;
    }

    int value;
    int status = da_pop(arr, &value);
    TEST_ASSERT_EQUAL_INT(-1, status, "da_pop on empty array should return -1");

    da_destroy(arr);
}

/* Clear 테스트 */
void test_clear(void) {
    DynamicArray *arr = da_create(0);
    if (arr == NULL) {
        printf("SKIP: test_clear (allocation failed)\n");
        return;
    }

    da_push(arr, 1);
    da_push(arr, 2);
    da_push(arr, 3);

    size_t old_capacity = da_capacity(arr);
    da_clear(arr);

    TEST_ASSERT_EQUAL_SIZE(0, da_size(arr), "Size should be 0 after clear");
    TEST_ASSERT(da_is_empty(arr), "Array should be empty after clear");
    TEST_ASSERT_EQUAL_SIZE(old_capacity, da_capacity(arr), "Capacity should not change after clear");

    da_destroy(arr);
}

/* NULL 포인터 테스트 */
void test_null_pointer_safety(void) {
    TEST_ASSERT_EQUAL_INT(-1, da_push(NULL, 10), "da_push with NULL should return -1");
    TEST_ASSERT_EQUAL_SIZE(0, da_size(NULL), "da_size with NULL should return 0");
    TEST_ASSERT(da_is_empty(NULL), "da_is_empty with NULL should return 1");

    /* da_destroy(NULL) should not crash */
    da_destroy(NULL);
    TEST_ASSERT(1, "da_destroy(NULL) should not crash");
}

int main(void) {
    printf("=== Running Dynamic Array Tests ===\n\n");

    test_create_and_destroy();
    test_create_with_capacity();
    test_push_single_element();
    test_push_multiple_elements();
    test_push_triggers_resize();
    test_get_and_set();
    test_get_out_of_bounds();
    test_pop();
    test_pop_empty_array();
    test_clear();
    test_null_pointer_safety();

    printf("\n=== Test Results ===\n");
    printf("Tests run: %d\n", tests_run);
    printf("Tests passed: %d\n", tests_passed);
    printf("Tests failed: %d\n", tests_run - tests_passed);

    return (tests_run == tests_passed) ? 0 : 1;
}
