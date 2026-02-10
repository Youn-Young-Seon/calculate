# CMakeLists.txt
cmake_minimum_required(VERSION 3.15)
project(MyProject C)

# C 표준 설정
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 컴파일 옵션
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

# 헤더 파일 경로
include_directories(include)

# 라이브러리 생성
add_library(calculator_lib STATIC
    src/calculator.c
)

# 실행 파일
add_executable(app
    src/main.c
)

target_link_libraries(app calculator_lib)

# 테스트 활성화
enable_testing()
add_subdirectory(tests)
