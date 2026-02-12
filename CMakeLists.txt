# CMakeLists.txt
cmake_minimum_required(VERSION 3.15)
project(MyProject C CXX)

# C 표준 설정
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# C++ 표준 설정
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 컴파일 옵션
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

# 헤더 파일 경로
include_directories(include)

# ====================================
# C 라이브러리 및 실행 파일
# ====================================
add_library(calculator_lib STATIC
    src/calculator.c
    src/dynamic_array.c
    src/csv_parser.c
    src/tcp_server.c
)

# C 실행 파일
add_executable(app
    src/main.c
)
target_link_libraries(app calculator_lib)

# C TCP 서버
add_executable(tcp_server
    src/server_main.c
)
target_link_libraries(tcp_server calculator_lib)

# Windows에서 Winsock 라이브러리 링크
if(WIN32)
    target_link_libraries(tcp_server ws2_32)
    target_link_libraries(calculator_lib ws2_32)
endif()

# ====================================
# C++ 라이브러리 및 실행 파일
# ====================================
add_library(calculator_lib_cpp STATIC
    src/cpp/Calculator.cpp
    src/cpp/DynamicArray.cpp
    src/cpp/CSVParser.cpp
    src/cpp/TCPServer.cpp
)

# C++ 실행 파일
add_executable(app_cpp
    src/cpp/main.cpp
)
target_link_libraries(app_cpp calculator_lib_cpp)

# C++ TCP 서버
add_executable(tcp_server_cpp
    src/cpp/server_main.cpp
)
target_link_libraries(tcp_server_cpp calculator_lib_cpp)

# Windows에서 Winsock 라이브러리 링크
if(WIN32)
    target_link_libraries(tcp_server_cpp ws2_32)
    target_link_libraries(calculator_lib_cpp ws2_32)
endif()

# 테스트 활성화
enable_testing()
add_subdirectory(tests)
