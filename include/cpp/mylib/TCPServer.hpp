#ifndef TCP_SERVER_HPP
#define TCP_SERVER_HPP

#include <string>

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

namespace mylib {

constexpr int DEFAULT_PORT = 8080;
constexpr int BUFFER_SIZE = 1024;

/**
 * @brief TCP 서버 클래스
 *
 * TCP 에코 서버를 구현합니다.
 * RAII 패턴으로 소켓 자동 관리를 제공합니다.
 */
class TCPServer {
private:
    SOCKET server_socket_;
    int port_;
    bool is_running_;
    bool is_initialized_;

    void handleClient(SOCKET client_socket);

public:
    /**
     * @brief 기본 생성자 - DEFAULT_PORT 사용
     */
    TCPServer();

    /**
     * @brief 포트를 지정하는 생성자
     * @param port 포트 번호
     */
    explicit TCPServer(int port);

    /**
     * @brief 소멸자 - 자동으로 서버 종료 및 리소스 정리
     */
    ~TCPServer();

    // 복사 생성자와 할당 연산자 삭제 (소켓은 복사 불가)
    TCPServer(const TCPServer&) = delete;
    TCPServer& operator=(const TCPServer&) = delete;

    // 이동 생성자와 할당 연산자
    TCPServer(TCPServer&& other) noexcept;
    TCPServer& operator=(TCPServer&& other) noexcept;

    /**
     * @brief 서버를 초기화합니다
     * @throws std::runtime_error 초기화 실패 시
     *
     * @note Windows에서는 WSAStartup()을 자동으로 호출합니다
     */
    void init();

    /**
     * @brief 서버를 시작하고 클라이언트 연결을 대기합니다
     * @throws std::runtime_error 서버 시작 실패 시
     *
     * @note 이 함수는 블로킹됩니다. Ctrl+C로 중단할 수 있습니다.
     */
    void start();

    /**
     * @brief 서버를 중지합니다
     */
    void stop();

    /**
     * @brief 서버가 실행 중인지 확인합니다
     * @return 실행 중이면 true
     */
    bool isRunning() const;

    /**
     * @brief 현재 포트를 반환합니다
     * @return 포트 번호
     */
    int getPort() const;
};

/**
 * @brief TCP 클라이언트 유틸리티
 *
 * 간단한 TCP 클라이언트 기능을 제공합니다.
 */
class TCPClient {
public:
    /**
     * @brief 서버에 연결하고 메시지를 전송합니다
     * @param host 서버 호스트 (예: "127.0.0.1")
     * @param port 서버 포트
     * @param message 전송할 메시지
     * @return 서버로부터 받은 응답
     * @throws std::runtime_error 연결 또는 전송 실패 시
     *
     * @note 테스트 및 간단한 클라이언트 구현용
     */
    static std::string send(const std::string& host, int port, const std::string& message);
};

} // namespace mylib

#endif // TCP_SERVER_HPP
