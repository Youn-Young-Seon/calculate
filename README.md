# Calculator Project

C/C++ 프로그래밍 학습을 위한 다중 유틸리티 라이브러리 프로젝트

이 프로젝트는 동일한 기능을 C와 C++로 각각 구현하여 두 언어의 차이점과 장단점을 비교 학습할 수 있도록 구성되었습니다.

## 프로젝트 구조

```
calculate/
├── include/
│   ├── mylib/          # C 헤더 파일
│   │   ├── calculator.h
│   │   ├── dynamic_array.h
│   │   ├── csv_parser.h
│   │   └── tcp_server.h
│   └── cpp/mylib/      # C++ 헤더 파일
│       ├── Calculator.hpp
│       ├── DynamicArray.hpp
│       ├── CSVParser.hpp
│       └── TCPServer.hpp
├── src/                # C 소스 파일
│   └── cpp/            # C++ 소스 파일
├── tests/              # C 테스트 코드
│   ├── cpp/            # C++ 테스트 코드
│   └── CMakeLists.txt
├── build/              # 빌드 출력
├── CMakeLists.txt      # CMake 설정
└── README.md
```

## 빌드 방법

### 방법 1: CMake 사용 (권장)

```bash
# Windows
cmake -B build -G "MinGW Makefiles"
cmake --build build

# C 버전 실행
.\build\app.exe

# C++ 버전 실행
.\build\app_cpp.exe

# 모든 테스트 실행
ctest --test-dir build --output-on-failure
```

### 방법 2: 직접 컴파일

#### C 버전
```bash
gcc -Wall -Wextra -std=c11 -I./include -c src/calculator.c -o build/calculator.o
gcc -Wall -Wextra -std=c11 -I./include -c src/main.c -o build/main.o
gcc build/calculator.o build/main.o -o build/app.exe
```

#### C++ 버전
```bash
g++ -Wall -Wextra -std=c++17 -I./include -c src/cpp/Calculator.cpp -o build/Calculator.o
g++ -Wall -Wextra -std=c++17 -I./include -c src/cpp/main.cpp -o build/main_cpp.o
g++ build/Calculator.o build/main_cpp.o -o build/app_cpp.exe
```

## 테스트 실행

```bash
# 모든 테스트 (C + C++)
ctest --test-dir build --output-on-failure

# C 테스트만
.\build\tests\test_calculator.exe
.\build\tests\test_dynamic_array.exe
.\build\tests\test_csv_parser.exe

# C++ 테스트만
.\build\tests\test_calculator_cpp.exe
.\build\tests\test_dynamic_array_cpp.exe
.\build\tests\test_csv_parser_cpp.exe
```

## 구현된 모듈

### 1. Calculator (계산기)
- 기본 산술 연산: 덧셈, 나눗셈, 평균 계산
- C: 함수 기반, 상태 코드 반환
- C++: 정적 메서드, 예외 처리

### 2. Dynamic Array (동적 배열)
- 자동 크기 조절 배열
- C: 수동 메모리 관리 (`malloc`/`free`)
- C++: RAII 패턴, `std::vector` 래퍼

### 3. CSV Parser (CSV 파서)
- CSV 파일 읽기/쓰기, 헤더 지원
- C: C 문자열 함수 사용
- C++: `std::string`, `std::vector` 사용

### 4. TCP Server (TCP 서버)
- 크로스 플랫폼 TCP 에코 서버
- C: 구조체 기반, 수동 소켓 관리
- C++: 클래스 기반, RAII 자동 소켓 관리

## 학습 포인트

### C 버전
- 수동 메모리 관리 (`malloc`, `free`, `realloc`)
- 포인터와 구조체 활용
- 헤더 가드와 분할 컴파일
- 상태 코드 기반 에러 처리
- 방어적 프로그래밍 (NULL 체크, 경계 검사)

### C++ 버전
- 객체 지향 프로그래밍 (클래스, 캡슐화)
- RAII 패턴 (자동 리소스 관리)
- STL 컨테이너 (`std::vector`, `std::string`)
- 예외 처리 (`try`-`catch`)
- 연산자 오버로딩
- 이동 의미론 (move semantics)
- Rule of Five

### 공통
- 크로스 플랫폼 빌드 (CMake)
- 단위 테스트 작성
- TCP/IP 네트워킹 (Winsock/BSD sockets)
- CSV 파일 파싱
- Doxygen 스타일 문서화

## C vs C++ 비교

이 프로젝트의 핵심 목적은 동일한 기능을 C와 C++로 구현하여 두 언어의 차이를 직접 비교하는 것입니다:

| 측면 | C | C++ |
|------|---|-----|
| 메모리 관리 | 수동 (`malloc`/`free`) | 자동 (RAII, 소멸자) |
| 에러 처리 | 상태 코드 반환 | 예외 (`throw`/`catch`) |
| 문자열 | `char*`, C 문자열 함수 | `std::string` |
| 배열 | 수동 관리 | `std::vector` |
| 코드 구성 | 함수, 구조체 | 클래스, 네임스페이스 |
| 타입 안전성 | 낮음 | 높음 (템플릿, 강타입) |
