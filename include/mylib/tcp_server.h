#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define SOCKET int
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket close
#endif

#define DEFAULT_PORT 8080
#define BUFFER_SIZE 1024

/**
 * @brief TCP 서버 구조체
 */
typedef struct {
    SOCKET server_socket;    /**< 서버 소켓 */
    int port;                /**< 포트 번호 */
    int is_running;          /**< 서버 실행 상태 */
} TCPServer;

/**
 * @brief TCP 서버를 초기화합니다
 * @param server 서버 구조체 포인터
 * @param port 포트 번호 (0이면 DEFAULT_PORT 사용)
 * @return 성공 시 0, 실패 시 -1
 *
 * @note Windows에서는 WSAStartup()을 자동으로 호출합니다
 */
int tcp_server_init(TCPServer *server, int port);

/**
 * @brief 서버를 시작하고 클라이언트 연결을 대기합니다
 * @param server 서버 구조체 포인터
 * @return 성공 시 0, 실패 시 -1
 *
 * @note 이 함수는 블로킹됩니다. Ctrl+C로 중단할 수 있습니다.
 */
int tcp_server_start(TCPServer *server);

/**
 * @brief 클라이언트 연결을 처리합니다 (에코 서버)
 * @param client_socket 클라이언트 소켓
 *
 * @note 클라이언트로부터 받은 데이터를 그대로 돌려보냅니다
 */
void tcp_server_handle_client(SOCKET client_socket);

/**
 * @brief 서버를 중지하고 리소스를 정리합니다
 * @param server 서버 구조체 포인터
 *
 * @note Windows에서는 WSACleanup()을 자동으로 호출합니다
 */
void tcp_server_shutdown(TCPServer *server);

/**
 * @brief 간단한 TCP 클라이언트로 서버에 연결합니다
 * @param host 서버 호스트 (예: "127.0.0.1")
 * @param port 서버 포트
 * @param message 전송할 메시지
 * @param response 응답을 저장할 버퍼
 * @param response_size 응답 버퍼 크기
 * @return 성공 시 0, 실패 시 -1
 *
 * @note 테스트 및 간단한 클라이언트 구현용
 */
int tcp_client_send(const char *host, int port, const char *message,
                    char *response, int response_size);

#endif /* TCP_SERVER_H */
