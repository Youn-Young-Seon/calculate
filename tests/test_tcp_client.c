#include <stdio.h>
#include <string.h>
#include "mylib/tcp_server.h"

int main(int argc, char *argv[]) {
    const char *host = "127.0.0.1";
    int port = DEFAULT_PORT;
    char response[BUFFER_SIZE];

    /* 커맨드 라인 인자 처리 */
    if (argc > 1) {
        host = argv[1];
    }
    if (argc > 2) {
        port = atoi(argv[2]);
    }

    printf("=== TCP Echo Client ===\n");
    printf("Connecting to %s:%d...\n\n", host, port);

    /* 테스트 메시지들 */
    const char *messages[] = {
        "Hello, Server!\n",
        "This is a test message.\n",
        "Echo server working?\n",
        "Goodbye!\n"
    };

    int num_messages = sizeof(messages) / sizeof(messages[0]);

    /* 각 메시지 전송 및 응답 확인 */
    for (int i = 0; i < num_messages; i++) {
        printf("--- Test %d ---\n", i + 1);

        memset(response, 0, sizeof(response));

        if (tcp_client_send(host, port, messages[i], response, sizeof(response)) == 0) {
            /* 에코 확인 */
            if (strcmp(messages[i], response) == 0) {
                printf("✓ Echo verified\n");
            } else {
                printf("✗ Echo mismatch!\n");
                printf("  Expected: %s", messages[i]);
                printf("  Got: %s", response);
            }
        } else {
            fprintf(stderr, "✗ Test failed\n");
        }

        printf("\n");

        /* 잠시 대기 */
#ifdef _WIN32
        Sleep(500);  /* Windows: milliseconds */
#else
        usleep(500000);  /* Unix: microseconds */
#endif
    }

    printf("All tests completed\n");

    return 0;
}
