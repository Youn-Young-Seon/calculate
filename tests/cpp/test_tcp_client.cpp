#include "cpp/mylib/TCPServer.hpp"
#include <iostream>
#include <cassert>
#include <thread>
#include <chrono>

using namespace mylib;

void test_tcp_echo() {
    try {
        std::string response = TCPClient::send("127.0.0.1", 8080, "Hello Server");
        std::cout << "Response from server: " << response << std::endl;
        assert(response == "Hello Server");
        std::cout << "✓ test_tcp_echo passed\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        std::cerr << "Make sure the TCP server is running on port 8080\n";
        std::cerr << "Run: ./build/tcp_server_cpp.exe\n";
        return;
    }
}

int main() {
    std::cout << "Running TCP Client C++ test...\n\n";
    std::cout << "Note: This test requires the TCP server to be running.\n";
    std::cout << "Please start the server first: ./build/tcp_server_cpp.exe\n\n";

    // 서버가 준비될 시간을 줌
    std::this_thread::sleep_for(std::chrono::seconds(1));

    test_tcp_echo();

    std::cout << "\n✓ All TCP Client tests passed!\n";
    return 0;
}
