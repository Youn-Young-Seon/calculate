#ifndef CSV_PARSER_H
#define CSV_PARSER_H

#include <stddef.h>  /* size_t */

/**
 * @brief CSV 데이터 구조체
 *
 * CSV 파일의 데이터를 메모리에 저장합니다.
 * 첫 번째 행은 헤더(컬럼 이름)로 간주됩니다.
 */
typedef struct {
    char **headers;      /**< 헤더 행 (컬럼 이름 배열) */
    char ***rows;        /**< 데이터 행들 (2차원 문자열 배열) */
    size_t row_count;    /**< 데이터 행 개수 (헤더 제외) */
    size_t col_count;    /**< 열(컬럼) 개수 */
} CSVData;

/**
 * @brief CSV 파일을 파싱합니다
 * @param filename 읽을 CSV 파일 경로
 * @return 파싱된 CSV 데이터, 실패 시 NULL
 *
 * @note 첫 번째 행은 헤더로 처리됩니다
 * @note 사용 후 반드시 csv_destroy()로 메모리를 해제해야 합니다
 *
 * @example
 * CSVData *data = csv_parse("data.csv");
 * if (data != NULL) {
 *     csv_print(data);
 *     csv_destroy(data);
 * }
 */
CSVData* csv_parse(const char *filename);

/**
 * @brief 특정 셀의 값을 가져옵니다
 * @param data CSV 데이터
 * @param row 행 인덱스 (0부터 시작)
 * @param col 열 인덱스 (0부터 시작)
 * @return 셀의 문자열 값, 실패 시 NULL
 *
 * @note 반환된 문자열은 내부 데이터를 가리키므로 수정하면 안 됩니다
 */
const char* csv_get(const CSVData *data, size_t row, size_t col);

/**
 * @brief 헤더 이름으로 열 인덱스를 찾습니다
 * @param data CSV 데이터
 * @param header_name 찾을 헤더 이름
 * @param col_index 결과를 저장할 포인터
 * @return 성공 시 0, 실패 시 -1 (헤더를 찾지 못함)
 */
int csv_find_column(const CSVData *data, const char *header_name, size_t *col_index);

/**
 * @brief 특정 행의 모든 값을 가져옵니다
 * @param data CSV 데이터
 * @param row 행 인덱스
 * @return 행의 문자열 배열, 실패 시 NULL
 *
 * @note 반환된 배열은 내부 데이터를 가리키므로 수정하면 안 됩니다
 */
char** csv_get_row(const CSVData *data, size_t row);

/**
 * @brief CSV 데이터를 콘솔에 출력합니다
 * @param data CSV 데이터
 *
 * @note 디버깅 및 확인용
 */
void csv_print(const CSVData *data);

/**
 * @brief CSV 데이터를 파일로 저장합니다
 * @param data CSV 데이터
 * @param filename 저장할 파일 경로
 * @return 성공 시 0, 실패 시 -1
 */
int csv_write(const CSVData *data, const char *filename);

/**
 * @brief CSV 데이터의 행 개수를 반환합니다
 * @param data CSV 데이터
 * @return 행 개수 (헤더 제외), data가 NULL이면 0
 */
size_t csv_row_count(const CSVData *data);

/**
 * @brief CSV 데이터의 열 개수를 반환합니다
 * @param data CSV 데이터
 * @return 열 개수, data가 NULL이면 0
 */
size_t csv_col_count(const CSVData *data);

/**
 * @brief CSV 데이터를 파괴하고 메모리를 해제합니다
 * @param data CSV 데이터
 *
 * @warning 이 함수 호출 후 data 포인터를 사용하면 안 됩니다
 */
void csv_destroy(CSVData *data);

#endif /* CSV_PARSER_H */
