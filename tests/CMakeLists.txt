# tests/CMakeLists.txt

# Unity 테스트 프레임워크가 있다면 사용
# 없으면 간단한 테스트 실행 파일만 생성

# ====================================
# C 테스트
# ====================================
add_executable(test_calculator
    test_calculator.c
)

target_link_libraries(test_calculator
    calculator_lib
)

# CTest 통합
add_test(NAME CalculatorTests COMMAND test_calculator)

# 동적 배열 테스트
add_executable(test_dynamic_array
    test_dynamic_array.c
)

target_link_libraries(test_dynamic_array
    calculator_lib
)

add_test(NAME DynamicArrayTests COMMAND test_dynamic_array)

# CSV 파서 테스트
add_executable(test_csv_parser
    test_csv_parser.c
)

target_link_libraries(test_csv_parser
    calculator_lib
)

add_test(NAME CSVParserTests COMMAND test_csv_parser)

# TCP 클라이언트 테스트
add_executable(test_tcp_client
    test_tcp_client.c
)

target_link_libraries(test_tcp_client
    calculator_lib
)

# Windows에서 Winsock 라이브러리 링크
if(WIN32)
    target_link_libraries(test_tcp_client ws2_32)
endif()

# 참고: TCP 테스트는 서버가 실행 중이어야 하므로 자동 테스트에서 제외

# ====================================
# C++ 테스트
# ====================================

# Calculator C++ 테스트
add_executable(test_calculator_cpp
    cpp/test_calculator.cpp
)
target_link_libraries(test_calculator_cpp calculator_lib_cpp)
add_test(NAME CalculatorCppTests COMMAND test_calculator_cpp)

# DynamicArray C++ 테스트
add_executable(test_dynamic_array_cpp
    cpp/test_dynamic_array.cpp
)
target_link_libraries(test_dynamic_array_cpp calculator_lib_cpp)
add_test(NAME DynamicArrayCppTests COMMAND test_dynamic_array_cpp)

# CSVParser C++ 테스트
add_executable(test_csv_parser_cpp
    cpp/test_csv_parser.cpp
)
target_link_libraries(test_csv_parser_cpp calculator_lib_cpp)
add_test(NAME CSVParserCppTests COMMAND test_csv_parser_cpp)

# TCP Client C++ 테스트
add_executable(test_tcp_client_cpp
    cpp/test_tcp_client.cpp
)
target_link_libraries(test_tcp_client_cpp calculator_lib_cpp)

# Windows에서 Winsock 라이브러리 링크
if(WIN32)
    target_link_libraries(test_tcp_client_cpp ws2_32)
endif()

# 참고: TCP C++ 테스트도 서버가 실행 중이어야 하므로 자동 테스트에서 제외
