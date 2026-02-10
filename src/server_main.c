#include <stdio.h>
#include <signal.h>
#include "mylib/tcp_server.h"

/* 전역 서버 변수 (시그널 핸들러에서 접근) */
static TCPServer g_server;

/**
 * @brief 시그널 핸들러 (Ctrl+C 처리)
 */
void signal_handler(int signal) {
    if (signal == SIGINT) {
        printf("\n\nShutting down server...\n");
        tcp_server_shutdown(&g_server);
        exit(0);
    }
}

int main(int argc, char *argv[]) {
    int port = DEFAULT_PORT;

    /* 커맨드 라인 인자로 포트 지정 가능 */
    if (argc > 1) {
        port = atoi(argv[1]);
        if (port <= 0 || port > 65535) {
            fprintf(stderr, "Invalid port number: %s\n", argv[1]);
            fprintf(stderr, "Usage: %s [port]\n", argv[0]);
            return 1;
        }
    }

    /* Ctrl+C 시그널 핸들러 등록 */
    signal(SIGINT, signal_handler);

    /* 서버 초기화 */
    if (tcp_server_init(&g_server, port) != 0) {
        fprintf(stderr, "Server initialization failed\n");
        return 1;
    }

    /* 서버 시작 */
    printf("=== TCP Echo Server ===\n");
    printf("Starting server on port %d...\n\n", port);

    if (tcp_server_start(&g_server) != 0) {
        fprintf(stderr, "Server start failed\n");
        tcp_server_shutdown(&g_server);
        return 1;
    }

    /* 정리 */
    tcp_server_shutdown(&g_server);

    return 0;
}
