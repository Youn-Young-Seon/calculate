# tests/CMakeLists.txt

# Unity 테스트 프레임워크가 있다면 사용
# 없으면 간단한 테스트 실행 파일만 생성

# 테스트 실행 파일
add_executable(test_calculator
    test_calculator.c
)

target_link_libraries(test_calculator
    calculator_lib
)

# CTest 통합
add_test(NAME CalculatorTests COMMAND test_calculator)
