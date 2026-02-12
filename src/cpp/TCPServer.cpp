#include "cpp/mylib/TCPServer.hpp"
#include <iostream>
#include <cstring>
#include <stdexcept>

#ifdef _WIN32
#pragma comment(lib, "ws2_32.lib")
#endif

namespace mylib {

TCPServer::TCPServer()
    : server_socket_(INVALID_SOCKET)
    , port_(DEFAULT_PORT)
    , is_running_(false)
    , is_initialized_(false) {
}

TCPServer::TCPServer(int port)
    : server_socket_(INVALID_SOCKET)
    , port_(port)
    , is_running_(false)
    , is_initialized_(false) {
}

TCPServer::~TCPServer() {
    stop();
}

TCPServer::TCPServer(TCPServer&& other) noexcept
    : server_socket_(other.server_socket_)
    , port_(other.port_)
    , is_running_(other.is_running_)
    , is_initialized_(other.is_initialized_) {
    other.server_socket_ = INVALID_SOCKET;
    other.is_running_ = false;
    other.is_initialized_ = false;
}

TCPServer& TCPServer::operator=(TCPServer&& other) noexcept {
    if (this != &other) {
        stop();

        server_socket_ = other.server_socket_;
        port_ = other.port_;
        is_running_ = other.is_running_;
        is_initialized_ = other.is_initialized_;

        other.server_socket_ = INVALID_SOCKET;
        other.is_running_ = false;
        other.is_initialized_ = false;
    }
    return *this;
}

void TCPServer::init() {
#ifdef _WIN32
    WSADATA wsa_data;
    int result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    if (result != 0) {
        throw std::runtime_error("WSAStartup failed: " + std::to_string(result));
    }
#endif

    server_socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket_ == INVALID_SOCKET) {
        throw std::runtime_error("Failed to create socket");
    }

    // SO_REUSEADDR 옵션 설정
    int opt = 1;
    if (setsockopt(server_socket_, SOL_SOCKET, SO_REUSEADDR,
                   reinterpret_cast<const char*>(&opt), sizeof(opt)) < 0) {
        closesocket(server_socket_);
        throw std::runtime_error("Failed to set socket options");
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port_);

    if (bind(server_socket_, reinterpret_cast<sockaddr*>(&server_addr),
             sizeof(server_addr)) == SOCKET_ERROR) {
        closesocket(server_socket_);
        throw std::runtime_error("Failed to bind socket");
    }

    if (listen(server_socket_, 5) == SOCKET_ERROR) {
        closesocket(server_socket_);
        throw std::runtime_error("Failed to listen on socket");
    }

    is_initialized_ = true;
    std::cout << "TCP Server initialized on port " << port_ << std::endl;
}

void TCPServer::start() {
    if (!is_initialized_) {
        init();
    }

    is_running_ = true;
    std::cout << "TCP Server started. Waiting for connections..." << std::endl;

    while (is_running_) {
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);

        SOCKET client_socket = accept(server_socket_,
                                      reinterpret_cast<sockaddr*>(&client_addr),
                                      &client_len);

        if (client_socket == INVALID_SOCKET) {
            if (is_running_) {
                std::cerr << "Failed to accept connection" << std::endl;
            }
            continue;
        }

        std::cout << "Client connected" << std::endl;
        handleClient(client_socket);
    }
}

void TCPServer::handleClient(SOCKET client_socket) {
    char buffer[BUFFER_SIZE];

    while (true) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);

        if (bytes_received <= 0) {
            break;
        }

        std::cout << "Received: " << buffer << std::endl;

        // 에코: 받은 데이터를 그대로 돌려보냄
        int bytes_sent = send(client_socket, buffer, bytes_received, 0);
        if (bytes_sent == SOCKET_ERROR) {
            std::cerr << "Failed to send data" << std::endl;
            break;
        }
    }

    closesocket(client_socket);
    std::cout << "Client disconnected" << std::endl;
}

void TCPServer::stop() {
    is_running_ = false;

    if (server_socket_ != INVALID_SOCKET) {
        closesocket(server_socket_);
        server_socket_ = INVALID_SOCKET;
    }

#ifdef _WIN32
    if (is_initialized_) {
        WSACleanup();
    }
#endif

    is_initialized_ = false;
    std::cout << "TCP Server stopped" << std::endl;
}

bool TCPServer::isRunning() const {
    return is_running_;
}

int TCPServer::getPort() const {
    return port_;
}

// TCPClient 구현
std::string TCPClient::send(const std::string& host, int port, const std::string& message) {
#ifdef _WIN32
    WSADATA wsa_data;
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        throw std::runtime_error("WSAStartup failed");
    }
#endif

    SOCKET client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == INVALID_SOCKET) {
#ifdef _WIN32
        WSACleanup();
#endif
        throw std::runtime_error("Failed to create socket");
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

#ifdef _WIN32
    server_addr.sin_addr.s_addr = inet_addr(host.c_str());
#else
    inet_pton(AF_INET, host.c_str(), &server_addr.sin_addr);
#endif

    if (connect(client_socket, reinterpret_cast<sockaddr*>(&server_addr),
                sizeof(server_addr)) == SOCKET_ERROR) {
        closesocket(client_socket);
#ifdef _WIN32
        WSACleanup();
#endif
        throw std::runtime_error("Failed to connect to server");
    }

    // 메시지 전송
    if (::send(client_socket, message.c_str(), message.length(), 0) == SOCKET_ERROR) {
        closesocket(client_socket);
#ifdef _WIN32
        WSACleanup();
#endif
        throw std::runtime_error("Failed to send data");
    }

    // 응답 받기
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    int bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);

    closesocket(client_socket);
#ifdef _WIN32
    WSACleanup();
#endif

    if (bytes_received <= 0) {
        throw std::runtime_error("Failed to receive response");
    }

    return std::string(buffer);
}

} // namespace mylib
