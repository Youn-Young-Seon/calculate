#include <stdio.h>
#include "mylib/calculator.h"

int main(void) {
    /* 왜 void?: C에서 main()과 main(void)는 다름 */

    printf("Addition: %d\n", add(5, 3));

    int result;
    if (divide(10, 2, &result) == 0) {
        printf("Division: %d\n", result);
    } else {
        fprintf(stderr, "Error: Division by zero\n");
        /* 왜 stderr?: 에러 메시지는 표준 에러 스트림으로 */
    }

    int numbers[] = {1, 2, 3, 4, 5};
    size_t count = sizeof(numbers) / sizeof(numbers[0]);
    /* 왜 이렇게?: 배열 크기를 자동으로 계산 (매직 넘버 방지) */

    printf("Average: %.2f\n", average(numbers, count));

    return 0;
    /* 왜 0?: 0은 성공, 비-0은 에러 (Unix 규약) */
}
