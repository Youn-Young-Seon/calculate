#include <stdio.h>
#include <assert.h>
#include "mylib/calculator.h"

/* 간단한 테스트 프레임워크 (Unity 대체) */
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

#define TEST_ASSERT_DOUBLE_WITHIN(tolerance, expected, actual, message) \
    do { \
        tests_run++; \
        double diff = (expected) - (actual); \
        if (diff < 0) diff = -diff; \
        if (diff <= (tolerance)) { \
            printf("PASS: %s\n", message); \
            tests_passed++; \
        } else { \
            printf("FAIL: %s (expected %.2f, got %.2f)\n", message, expected, actual); \
        } \
    } while(0)

/* 기본 기능 테스트 */
void test_add_positive_numbers(void) {
    TEST_ASSERT_EQUAL_INT(8, add(5, 3), "add(5, 3) should be 8");
    TEST_ASSERT_EQUAL_INT(0, add(0, 0), "add(0, 0) should be 0");
}

void test_add_negative_numbers(void) {
    TEST_ASSERT_EQUAL_INT(-8, add(-5, -3), "add(-5, -3) should be -8");
    TEST_ASSERT_EQUAL_INT(2, add(5, -3), "add(5, -3) should be 2");
}

/* 경계값 테스트 */
void test_divide_normal_case(void) {
    int result;
    int status = divide(10, 2, &result);

    TEST_ASSERT_EQUAL_INT(0, status, "divide should return 0 on success");
    TEST_ASSERT_EQUAL_INT(5, result, "10 / 2 should be 5");
}

void test_divide_by_zero(void) {
    int result;
    int status = divide(10, 0, &result);

    TEST_ASSERT_EQUAL_INT(-1, status, "divide by zero should return -1");
}

void test_divide_null_result(void) {
    int status = divide(10, 2, NULL);
    TEST_ASSERT_EQUAL_INT(0, status, "divide with NULL result should not crash");
}

/* 배열 테스트 */
void test_average_normal_array(void) {
    int arr[] = {1, 2, 3, 4, 5};
    double result = average(arr, 5);

    TEST_ASSERT_DOUBLE_WITHIN(0.01, 3.0, result, "average of {1,2,3,4,5} should be 3.0");
}

void test_average_null_array(void) {
    double result = average(NULL, 5);
    TEST_ASSERT_DOUBLE_WITHIN(0.01, 0.0, result, "average of NULL should be 0.0");
}

void test_average_zero_size(void) {
    int arr[] = {1, 2, 3};
    double result = average(arr, 0);
    TEST_ASSERT_DOUBLE_WITHIN(0.01, 0.0, result, "average of size 0 should be 0.0");
}

int main(void) {
    printf("=== Running Calculator Tests ===\n\n");

    test_add_positive_numbers();
    test_add_negative_numbers();
    test_divide_normal_case();
    test_divide_by_zero();
    test_divide_null_result();
    test_average_normal_array();
    test_average_null_array();
    test_average_zero_size();

    printf("\n=== Test Results ===\n");
    printf("Tests run: %d\n", tests_run);
    printf("Tests passed: %d\n", tests_passed);
    printf("Tests failed: %d\n", tests_run - tests_passed);

    return (tests_run == tests_passed) ? 0 : 1;
}
