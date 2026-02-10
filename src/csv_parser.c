#include "mylib/csv_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 4096
#define INITIAL_ROW_CAPACITY 16

/**
 * @brief 문자열을 복사합니다 (strdup 대체)
 */
static char* string_duplicate(const char *str) {
    if (str == NULL) {
        return NULL;
    }

    size_t len = strlen(str);
    char *dup = (char *)malloc(len + 1);
    if (dup == NULL) {
        return NULL;
    }

    memcpy(dup, str, len + 1);
    return dup;
}

/**
 * @brief 문자열 끝의 개행 문자를 제거합니다
 */
static void trim_newline(char *str) {
    if (str == NULL) {
        return;
    }

    size_t len = strlen(str);
    while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r')) {
        str[len - 1] = '\0';
        len--;
    }
}

/**
 * @brief CSV 행을 파싱하여 필드 배열로 변환합니다
 * @param line 파싱할 행
 * @param field_count 필드 개수를 저장할 포인터
 * @return 필드 배열, 실패 시 NULL
 */
static char** parse_csv_line(const char *line, size_t *field_count) {
    if (line == NULL || field_count == NULL) {
        return NULL;
    }

    size_t capacity = 8;
    char **fields = (char **)malloc(capacity * sizeof(char *));
    if (fields == NULL) {
        return NULL;
    }

    *field_count = 0;
    const char *ptr = line;
    char buffer[MAX_LINE_LENGTH];
    size_t buf_pos = 0;
    int in_quotes = 0;

    while (*ptr != '\0') {
        if (*ptr == '"') {
            /* 따옴표 토글 */
            in_quotes = !in_quotes;
            ptr++;
        } else if (*ptr == ',' && !in_quotes) {
            /* 필드 구분자 */
            buffer[buf_pos] = '\0';

            /* 용량 확장 */
            if (*field_count >= capacity) {
                capacity *= 2;
                char **new_fields = (char **)realloc(fields, capacity * sizeof(char *));
                if (new_fields == NULL) {
                    /* 메모리 할당 실패 - 정리 */
                    for (size_t i = 0; i < *field_count; i++) {
                        free(fields[i]);
                    }
                    free(fields);
                    return NULL;
                }
                fields = new_fields;
            }

            fields[*field_count] = string_duplicate(buffer);
            if (fields[*field_count] == NULL) {
                /* 메모리 할당 실패 - 정리 */
                for (size_t i = 0; i < *field_count; i++) {
                    free(fields[i]);
                }
                free(fields);
                return NULL;
            }
            (*field_count)++;
            buf_pos = 0;
            ptr++;
        } else {
            /* 일반 문자 */
            if (buf_pos < MAX_LINE_LENGTH - 1) {
                buffer[buf_pos++] = *ptr;
            }
            ptr++;
        }
    }

    /* 마지막 필드 추가 */
    buffer[buf_pos] = '\0';

    if (*field_count >= capacity) {
        capacity++;
        char **new_fields = (char **)realloc(fields, capacity * sizeof(char *));
        if (new_fields == NULL) {
            for (size_t i = 0; i < *field_count; i++) {
                free(fields[i]);
            }
            free(fields);
            return NULL;
        }
        fields = new_fields;
    }

    fields[*field_count] = string_duplicate(buffer);
    if (fields[*field_count] == NULL) {
        for (size_t i = 0; i < *field_count; i++) {
            free(fields[i]);
        }
        free(fields);
        return NULL;
    }
    (*field_count)++;

    return fields;
}

CSVData* csv_parse(const char *filename) {
    if (filename == NULL) {
        return NULL;
    }

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return NULL;
    }

    CSVData *data = (CSVData *)malloc(sizeof(CSVData));
    if (data == NULL) {
        fclose(file);
        return NULL;
    }

    data->headers = NULL;
    data->rows = NULL;
    data->row_count = 0;
    data->col_count = 0;

    char line[MAX_LINE_LENGTH];
    int is_first_line = 1;
    size_t row_capacity = INITIAL_ROW_CAPACITY;

    data->rows = (char ***)malloc(row_capacity * sizeof(char **));
    if (data->rows == NULL) {
        free(data);
        fclose(file);
        return NULL;
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        trim_newline(line);

        /* 빈 줄 건너뛰기 */
        if (strlen(line) == 0) {
            continue;
        }

        size_t field_count;
        char **fields = parse_csv_line(line, &field_count);

        if (fields == NULL) {
            csv_destroy(data);
            fclose(file);
            return NULL;
        }

        if (is_first_line) {
            /* 헤더 행 */
            data->headers = fields;
            data->col_count = field_count;
            is_first_line = 0;
        } else {
            /* 데이터 행 */
            if (field_count != data->col_count) {
                /* 열 개수 불일치 - 무시하거나 에러 처리 */
                for (size_t i = 0; i < field_count; i++) {
                    free(fields[i]);
                }
                free(fields);
                continue;
            }

            /* 용량 확장 */
            if (data->row_count >= row_capacity) {
                row_capacity *= 2;
                char ***new_rows = (char ***)realloc(data->rows, row_capacity * sizeof(char **));
                if (new_rows == NULL) {
                    for (size_t i = 0; i < field_count; i++) {
                        free(fields[i]);
                    }
                    free(fields);
                    csv_destroy(data);
                    fclose(file);
                    return NULL;
                }
                data->rows = new_rows;
            }

            data->rows[data->row_count] = fields;
            data->row_count++;
        }
    }

    fclose(file);
    return data;
}

const char* csv_get(const CSVData *data, size_t row, size_t col) {
    if (data == NULL || row >= data->row_count || col >= data->col_count) {
        return NULL;
    }

    return data->rows[row][col];
}

int csv_find_column(const CSVData *data, const char *header_name, size_t *col_index) {
    if (data == NULL || header_name == NULL || col_index == NULL) {
        return -1;
    }

    for (size_t i = 0; i < data->col_count; i++) {
        if (strcmp(data->headers[i], header_name) == 0) {
            *col_index = i;
            return 0;
        }
    }

    return -1;  /* 헤더를 찾지 못함 */
}

char** csv_get_row(const CSVData *data, size_t row) {
    if (data == NULL || row >= data->row_count) {
        return NULL;
    }

    return data->rows[row];
}

void csv_print(const CSVData *data) {
    if (data == NULL) {
        printf("(NULL CSV data)\n");
        return;
    }

    /* 헤더 출력 */
    printf("Headers: ");
    for (size_t i = 0; i < data->col_count; i++) {
        printf("%s", data->headers[i]);
        if (i < data->col_count - 1) {
            printf(", ");
        }
    }
    printf("\n");

    /* 데이터 출력 */
    printf("\nData (%zu rows):\n", data->row_count);
    for (size_t i = 0; i < data->row_count; i++) {
        printf("Row %zu: ", i);
        for (size_t j = 0; j < data->col_count; j++) {
            printf("%s", data->rows[i][j]);
            if (j < data->col_count - 1) {
                printf(", ");
            }
        }
        printf("\n");
    }
}

int csv_write(const CSVData *data, const char *filename) {
    if (data == NULL || filename == NULL) {
        return -1;
    }

    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        return -1;
    }

    /* 헤더 쓰기 */
    for (size_t i = 0; i < data->col_count; i++) {
        fprintf(file, "%s", data->headers[i]);
        if (i < data->col_count - 1) {
            fprintf(file, ",");
        }
    }
    fprintf(file, "\n");

    /* 데이터 쓰기 */
    for (size_t i = 0; i < data->row_count; i++) {
        for (size_t j = 0; j < data->col_count; j++) {
            fprintf(file, "%s", data->rows[i][j]);
            if (j < data->col_count - 1) {
                fprintf(file, ",");
            }
        }
        fprintf(file, "\n");
    }

    fclose(file);
    return 0;
}

size_t csv_row_count(const CSVData *data) {
    if (data == NULL) {
        return 0;
    }
    return data->row_count;
}

size_t csv_col_count(const CSVData *data) {
    if (data == NULL) {
        return 0;
    }
    return data->col_count;
}

void csv_destroy(CSVData *data) {
    if (data == NULL) {
        return;
    }

    /* 헤더 해제 */
    if (data->headers != NULL) {
        for (size_t i = 0; i < data->col_count; i++) {
            free(data->headers[i]);
        }
        free(data->headers);
    }

    /* 데이터 행 해제 */
    if (data->rows != NULL) {
        for (size_t i = 0; i < data->row_count; i++) {
            if (data->rows[i] != NULL) {
                for (size_t j = 0; j < data->col_count; j++) {
                    free(data->rows[i][j]);
                }
                free(data->rows[i]);
            }
        }
        free(data->rows);
    }

    /* 구조체 해제 */
    free(data);
}
