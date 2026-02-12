#include "cpp/mylib/TCPServer.hpp"
#include <iostream>
#include <exception>

using namespace mylib;

int main() {
    std::cout << "=== C++ TCP Echo Server ===\n\n";

    try {
        TCPServer server(8080);
        server.init();
        server.start();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
