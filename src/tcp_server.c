#include "mylib/tcp_server.h"
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#pragma comment(lib, "ws2_32.lib")
#endif

/**
 * @brief Winsock 초기화 (Windows 전용)
 */
static int init_winsock(void) {
#ifdef _WIN32
    WSADATA wsa_data;
    int result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    if (result != 0) {
        fprintf(stderr, "WSAStartup failed: %d\n", result);
        return -1;
    }
#endif
    return 0;
}

/**
 * @brief Winsock 정리 (Windows 전용)
 */
static void cleanup_winsock(void) {
#ifdef _WIN32
    WSACleanup();
#endif
}

int tcp_server_init(TCPServer *server, int port) {
    if (server == NULL) {
        return -1;
    }

    /* Winsock 초기화 */
    if (init_winsock() != 0) {
        return -1;
    }

    /* 포트 설정 */
    server->port = (port > 0) ? port : DEFAULT_PORT;
    server->is_running = 0;

    /* 소켓 생성 */
    server->server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server->server_socket == INVALID_SOCKET) {
        fprintf(stderr, "Socket creation failed\n");
        cleanup_winsock();
        return -1;
    }

    /* 주소 재사용 옵션 설정 */
    int opt = 1;
#ifdef _WIN32
    setsockopt(server->server_socket, SOL_SOCKET, SO_REUSEADDR,
               (char *)&opt, sizeof(opt));
#else
    setsockopt(server->server_socket, SOL_SOCKET, SO_REUSEADDR,
               &opt, sizeof(opt));
#endif

    /* 서버 주소 설정 */
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;  /* 모든 인터페이스에서 수신 */
    server_addr.sin_port = htons(server->port);

    /* 소켓을 주소에 바인드 */
    if (bind(server->server_socket, (struct sockaddr *)&server_addr,
             sizeof(server_addr)) == SOCKET_ERROR) {
        fprintf(stderr, "Bind failed on port %d\n", server->port);
        closesocket(server->server_socket);
        cleanup_winsock();
        return -1;
    }

    /* 연결 대기 */
    if (listen(server->server_socket, 5) == SOCKET_ERROR) {
        fprintf(stderr, "Listen failed\n");
        closesocket(server->server_socket);
        cleanup_winsock();
        return -1;
    }

    printf("TCP Server initialized on port %d\n", server->port);
    return 0;
}

int tcp_server_start(TCPServer *server) {
    if (server == NULL) {
        return -1;
    }

    server->is_running = 1;
    printf("Server is listening on port %d...\n", server->port);
    printf("Press Ctrl+C to stop the server\n\n");

    while (server->is_running) {
        struct sockaddr_in client_addr;
        int client_addr_len = sizeof(client_addr);

        /* 클라이언트 연결 수락 */
        SOCKET client_socket = accept(server->server_socket,
                                      (struct sockaddr *)&client_addr,
                                      &client_addr_len);

        if (client_socket == INVALID_SOCKET) {
            fprintf(stderr, "Accept failed\n");
            continue;
        }

        /* 클라이언트 정보 출력 */
        char client_ip[INET_ADDRSTRLEN];
#ifdef _WIN32
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
#else
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
#endif
        printf("Client connected: %s:%d\n", client_ip, ntohs(client_addr.sin_port));

        /* 클라이언트 처리 */
        tcp_server_handle_client(client_socket);

        /* 소켓 닫기 */
        closesocket(client_socket);
        printf("Client disconnected: %s:%d\n\n", client_ip, ntohs(client_addr.sin_port));
    }

    return 0;
}

void tcp_server_handle_client(SOCKET client_socket) {
    char buffer[BUFFER_SIZE];
    int bytes_received;

    while (1) {
        /* 클라이언트로부터 데이터 수신 */
        bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);

        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';  /* NULL 종료 */
            printf("Received: %s", buffer);

            /* 에코: 받은 데이터를 그대로 전송 */
            int bytes_sent = send(client_socket, buffer, bytes_received, 0);
            if (bytes_sent == SOCKET_ERROR) {
                fprintf(stderr, "Send failed\n");
                break;
            }
            printf("Echoed: %d bytes\n", bytes_sent);

        } else if (bytes_received == 0) {
            /* 클라이언트가 연결을 종료함 */
            printf("Client closed connection\n");
            break;
        } else {
            /* 에러 발생 */
            fprintf(stderr, "Recv failed\n");
            break;
        }
    }
}

void tcp_server_shutdown(TCPServer *server) {
    if (server == NULL) {
        return;
    }

    server->is_running = 0;

    if (server->server_socket != INVALID_SOCKET) {
        closesocket(server->server_socket);
        server->server_socket = INVALID_SOCKET;
    }

    cleanup_winsock();
    printf("Server shutdown complete\n");
}

int tcp_client_send(const char *host, int port, const char *message,
                    char *response, int response_size) {
    if (host == NULL || message == NULL || response == NULL) {
        return -1;
    }

    /* Winsock 초기화 */
    if (init_winsock() != 0) {
        return -1;
    }

    /* 소켓 생성 */
    SOCKET client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (client_socket == INVALID_SOCKET) {
        fprintf(stderr, "Client socket creation failed\n");
        cleanup_winsock();
        return -1;
    }

    /* 서버 주소 설정 */
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

#ifdef _WIN32
    inet_pton(AF_INET, host, &server_addr.sin_addr);
#else
    inet_pton(AF_INET, host, &server_addr.sin_addr);
#endif

    /* 서버에 연결 */
    if (connect(client_socket, (struct sockaddr *)&server_addr,
                sizeof(server_addr)) == SOCKET_ERROR) {
        fprintf(stderr, "Connection failed to %s:%d\n", host, port);
        closesocket(client_socket);
        cleanup_winsock();
        return -1;
    }

    printf("Connected to %s:%d\n", host, port);

    /* 메시지 전송 */
    int bytes_sent = send(client_socket, message, (int)strlen(message), 0);
    if (bytes_sent == SOCKET_ERROR) {
        fprintf(stderr, "Send failed\n");
        closesocket(client_socket);
        cleanup_winsock();
        return -1;
    }

    printf("Sent: %s", message);

    /* 응답 수신 */
    int bytes_received = recv(client_socket, response, response_size - 1, 0);
    if (bytes_received > 0) {
        response[bytes_received] = '\0';
        printf("Received: %s", response);
    } else if (bytes_received == 0) {
        printf("Server closed connection\n");
    } else {
        fprintf(stderr, "Recv failed\n");
        closesocket(client_socket);
        cleanup_winsock();
        return -1;
    }

    /* 정리 */
    closesocket(client_socket);
    cleanup_winsock();

    return 0;
}
