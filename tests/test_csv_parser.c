#include <stdio.h>
#include <string.h>
#include "mylib/csv_parser.h"

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

#define TEST_ASSERT_STRING_EQUAL(expected, actual, message) \
    do { \
        tests_run++; \
        if ((expected) != NULL && (actual) != NULL && strcmp(expected, actual) == 0) { \
            printf("PASS: %s\n", message); \
            tests_passed++; \
        } else { \
            printf("FAIL: %s (expected \"%s\", got \"%s\")\n", message, \
                   (expected) ? expected : "NULL", (actual) ? actual : "NULL"); \
        } \
    } while(0)

/* 파일 파싱 테스트 */
void test_parse_csv_file(void) {
    CSVData *data = csv_parse("tests/test_data.csv");
    TEST_ASSERT(data != NULL, "csv_parse should return non-NULL");

    if (data != NULL) {
        TEST_ASSERT_EQUAL_SIZE(5, csv_row_count(data), "Should have 5 data rows");
        TEST_ASSERT_EQUAL_SIZE(4, csv_col_count(data), "Should have 4 columns");
        csv_destroy(data);
    }
}

/* 헤더 테스트 */
void test_headers(void) {
    CSVData *data = csv_parse("tests/test_data.csv");
    if (data == NULL) {
        printf("SKIP: test_headers (file not found)\n");
        return;
    }

    TEST_ASSERT_STRING_EQUAL("name", data->headers[0], "First header should be 'name'");
    TEST_ASSERT_STRING_EQUAL("age", data->headers[1], "Second header should be 'age'");
    TEST_ASSERT_STRING_EQUAL("city", data->headers[2], "Third header should be 'city'");
    TEST_ASSERT_STRING_EQUAL("score", data->headers[3], "Fourth header should be 'score'");

    csv_destroy(data);
}

/* 데이터 읽기 테스트 */
void test_read_data(void) {
    CSVData *data = csv_parse("tests/test_data.csv");
    if (data == NULL) {
        printf("SKIP: test_read_data (file not found)\n");
        return;
    }

    /* 첫 번째 행 */
    const char *name = csv_get(data, 0, 0);
    TEST_ASSERT_STRING_EQUAL("Alice", name, "Row 0, Col 0 should be 'Alice'");

    const char *age = csv_get(data, 0, 1);
    TEST_ASSERT_STRING_EQUAL("25", age, "Row 0, Col 1 should be '25'");

    /* 마지막 행 */
    const char *last_name = csv_get(data, 4, 0);
    TEST_ASSERT_STRING_EQUAL("Eve", last_name, "Row 4, Col 0 should be 'Eve'");

    const char *last_score = csv_get(data, 4, 3);
    TEST_ASSERT_STRING_EQUAL("91.2", last_score, "Row 4, Col 3 should be '91.2'");

    csv_destroy(data);
}

/* 범위 초과 테스트 */
void test_out_of_bounds(void) {
    CSVData *data = csv_parse("tests/test_data.csv");
    if (data == NULL) {
        printf("SKIP: test_out_of_bounds (file not found)\n");
        return;
    }

    const char *invalid = csv_get(data, 100, 0);
    TEST_ASSERT(invalid == NULL, "Out of bounds row should return NULL");

    invalid = csv_get(data, 0, 100);
    TEST_ASSERT(invalid == NULL, "Out of bounds column should return NULL");

    csv_destroy(data);
}

/* 열 찾기 테스트 */
void test_find_column(void) {
    CSVData *data = csv_parse("tests/test_data.csv");
    if (data == NULL) {
        printf("SKIP: test_find_column (file not found)\n");
        return;
    }

    size_t col_index;
    int status = csv_find_column(data, "city", &col_index);

    TEST_ASSERT_EQUAL_INT(0, status, "Should find 'city' column");
    TEST_ASSERT_EQUAL_SIZE(2, col_index, "'city' should be at index 2");

    status = csv_find_column(data, "nonexistent", &col_index);
    TEST_ASSERT_EQUAL_INT(-1, status, "Should not find 'nonexistent' column");

    csv_destroy(data);
}

/* 행 가져오기 테스트 */
void test_get_row(void) {
    CSVData *data = csv_parse("tests/test_data.csv");
    if (data == NULL) {
        printf("SKIP: test_get_row (file not found)\n");
        return;
    }

    char **row = csv_get_row(data, 1);  /* Bob의 행 */
    TEST_ASSERT(row != NULL, "csv_get_row should return non-NULL");

    if (row != NULL) {
        TEST_ASSERT_STRING_EQUAL("Bob", row[0], "Row 1, Col 0 should be 'Bob'");
        TEST_ASSERT_STRING_EQUAL("30", row[1], "Row 1, Col 1 should be '30'");
        TEST_ASSERT_STRING_EQUAL("Busan", row[2], "Row 1, Col 2 should be 'Busan'");
    }

    csv_destroy(data);
}

/* 파일 쓰기 테스트 */
void test_write_csv(void) {
    CSVData *data = csv_parse("tests/test_data.csv");
    if (data == NULL) {
        printf("SKIP: test_write_csv (file not found)\n");
        return;
    }

    int status = csv_write(data, "tests/test_output.csv");
    TEST_ASSERT_EQUAL_INT(0, status, "csv_write should return 0 on success");

    /* 쓴 파일 다시 읽기 */
    CSVData *written_data = csv_parse("tests/test_output.csv");
    TEST_ASSERT(written_data != NULL, "Should be able to parse written file");

    if (written_data != NULL) {
        TEST_ASSERT_EQUAL_SIZE(data->row_count, written_data->row_count,
                               "Written file should have same row count");
        TEST_ASSERT_EQUAL_SIZE(data->col_count, written_data->col_count,
                               "Written file should have same column count");
        csv_destroy(written_data);
    }

    csv_destroy(data);
}

/* NULL 포인터 안전성 테스트 */
void test_null_pointer_safety(void) {
    CSVData *data = csv_parse(NULL);
    TEST_ASSERT(data == NULL, "csv_parse with NULL should return NULL");

    TEST_ASSERT(csv_get(NULL, 0, 0) == NULL, "csv_get with NULL should return NULL");
    TEST_ASSERT_EQUAL_SIZE(0, csv_row_count(NULL), "csv_row_count with NULL should return 0");
    TEST_ASSERT_EQUAL_SIZE(0, csv_col_count(NULL), "csv_col_count with NULL should return 0");

    /* csv_destroy(NULL) should not crash */
    csv_destroy(NULL);
    TEST_ASSERT(1, "csv_destroy(NULL) should not crash");
}

/* 존재하지 않는 파일 테스트 */
void test_nonexistent_file(void) {
    CSVData *data = csv_parse("nonexistent_file.csv");
    TEST_ASSERT(data == NULL, "csv_parse with nonexistent file should return NULL");
}

int main(void) {
    printf("=== Running CSV Parser Tests ===\n\n");

    test_parse_csv_file();
    test_headers();
    test_read_data();
    test_out_of_bounds();
    test_find_column();
    test_get_row();
    test_write_csv();
    test_null_pointer_safety();
    test_nonexistent_file();

    printf("\n=== Test Results ===\n");
    printf("Tests run: %d\n", tests_run);
    printf("Tests passed: %d\n", tests_passed);
    printf("Tests failed: %d\n", tests_run - tests_passed);

    return (tests_run == tests_passed) ? 0 : 1;
}
