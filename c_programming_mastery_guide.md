# C언어 숙련도 향상을 위한 종합 가이드

## 📋 학습 목표 및 전제조건

### 현재 상태
- ✅ "독하게 시작하는 C 프로그래밍" 서적 및 강의 완료
- ✅ 기본 문법, 포인터, 메모리 개념 이해 완료
- 🎯 목표: 실무 수준의 C 프로그래밍 능력 습득

### 개발 환경
- **IDE**: Visual Studio Code
- **OS**: Windows (주 환경)
- **컴파일러**: GCC (MinGW-w64), MSVC
- **빌드 시스템**: Make, CMake

---

## 🛠️ 개발 환경 설정

### Windows 환경 구성

```bash
# 필수 도구 설치 (PowerShell 관리자 권한)
# 1. MinGW-w64 설치 (GCC 컴파일러)
winget install -e --id=MSYS2.MSYS2

# 2. CMake 설치
winget install -e --id=Kitware.CMake

# 3. Git 설치
winget install -e --id=Git.Git
```

### VSCode 확장 프로그램

```json
{
  "recommendations": [
    "ms-vscode.cpptools",           // C/C++ IntelliSense
    "ms-vscode.cmake-tools",        // CMake 지원
    "twxs.cmake",                   // CMake 언어 지원
    "ms-vscode.makefile-tools",     // Makefile 지원
    "vadimcn.vscode-lldb",          // 디버거
    "hbenl.vscode-test-explorer"   // 테스트 탐색기
  ]
}
```

### 다른 OS 사용 시 차이점

| 항목 | Windows | Linux/macOS | 설명 |
|------|---------|-------------|------|
| **컴파일러** | MinGW-w64, MSVC | GCC, Clang | Linux/macOS는 기본 제공 |
| **경로 구분자** | `\` (백슬래시) | `/` (슬래시) | 코드에서는 `/` 권장 |
| **실행 파일** | `.exe` | 확장자 없음 | 빌드 스크립트에서 처리 |
| **라이브러리** | `.dll`, `.lib` | `.so`, `.a` | 동적/정적 라이브러리 형식 |
| **개행 문자** | `\r\n` (CRLF) | `\n` (LF) | Git에서 자동 변환 설정 |
| **시스템 API** | WinAPI | POSIX API | 플랫폼 독립적 코드 작성 필요 |

---

## 📁 프로젝트 구조 (Best Practice)

```
my_c_project/
├── .vscode/                    # VSCode 설정
│   ├── c_cpp_properties.json   # IntelliSense 설정
│   ├── launch.json             # 디버그 설정
│   └── tasks.json              # 빌드 태스크
├── build/                      # 빌드 출력 (git ignore)
├── docs/                       # 문서
│   └── API.md
├── include/                    # 공개 헤더 파일
│   └── mylib/
│       └── calculator.h
├── src/                        # 소스 파일
│   ├── calculator.c
│   └── main.c
├── tests/                      # 테스트 코드
│   ├── test_calculator.c
│   └── test_main.c
├── third_party/                # 외부 라이브러리
│   └── unity/                  # 테스트 프레임워크
├── .gitignore
├── CMakeLists.txt              # CMake 빌드 설정
├── Makefile                    # Make 빌드 설정
└── README.md
```

### 구조 설계 원칙

1. **관심사 분리**: 헤더(`include/`), 구현(`src/`), 테스트(`tests/`) 분리
2. **빌드 산출물 격리**: `build/` 디렉토리에 모든 생성 파일 집중
3. **플랫폼 독립성**: 플랫폼별 코드는 조건부 컴파일로 처리
4. **재사용성**: 라이브러리 형태로 모듈화

---

## 🔨 빌드 방법 (4가지 방식)

### 1. 직접 컴파일 (수동)

```bash
# Windows (MinGW)
gcc -Wall -Wextra -std=c11 -I./include -c src/calculator.c -o build/calculator.o
gcc -Wall -Wextra -std=c11 -I./include -c src/main.c -o build/main.o
gcc build/calculator.o build/main.o -o build/app.exe

# Linux/macOS
gcc -Wall -Wextra -std=c11 -I./include -c src/calculator.c -o build/calculator.o
gcc -Wall -Wextra -std=c11 -I./include -c src/main.c -o build/main.o
gcc build/calculator.o build/main.o -o build/app
```

**설명**:
- `-Wall -Wextra`: 모든 경고 활성화 (코드 품질 향상)
- `-std=c11`: C11 표준 사용
- `-I./include`: 헤더 파일 검색 경로
- `-c`: 오브젝트 파일만 생성 (링크 안 함)
- `-o`: 출력 파일 지정

**왜 이렇게?**: 컴파일 과정을 명확히 이해. 대규모 프로젝트에는 비효율적.

### 2. Makefile 사용

```makefile
# Makefile
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -I./include
LDFLAGS = 

# 플랫폼별 설정
ifeq ($(OS),Windows_NT)
    EXECUTABLE = build/app.exe
    RM = del /Q
    MKDIR = if not exist build mkdir build
else
    EXECUTABLE = build/app
    RM = rm -f
    MKDIR = mkdir -p build
endif

SRCS = src/calculator.c src/main.c
OBJS = $(SRCS:src/%.c=build/%.o)

.PHONY: all clean test

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

build/%.o: src/%.c
	@$(MKDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) build\*.o $(EXECUTABLE) 2>nul || true

test: all
	$(EXECUTABLE)
```

**사용법**:
```bash
make          # 빌드
make clean    # 정리
make test     # 테스트 실행
```

**왜 이렇게?**: 
- 변경된 파일만 재컴파일 (증분 빌드)
- 의존성 자동 관리
- 반복 작업 자동화

### 3. CMake 사용 (권장)

```cmake
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
```

**사용법**:
```bash
# Windows
cmake -B build -G "MinGW Makefiles"
cmake --build build

# Linux/macOS
cmake -B build
cmake --build build

# 실행
./build/app  # Linux/macOS
.\build\app.exe  # Windows
```

**왜 CMake?**:
- 크로스 플랫폼 지원 (Windows, Linux, macOS)
- 복잡한 의존성 관리
- IDE 프로젝트 파일 자동 생성
- 현대적 C/C++ 프로젝트 표준

### 4. VSCode Tasks 사용

`.vscode/tasks.json`:
```json
{
  "version": "2.0.0",
  "tasks": [
    {
      "label": "Build with CMake",
      "type": "shell",
      "command": "cmake",
      "args": [
        "--build",
        "build"
      ],
      "group": {
        "kind": "build",
        "isDefault": true
      },
      "problemMatcher": ["$gcc"]
    },
    {
      "label": "Configure CMake",
      "type": "shell",
      "command": "cmake",
      "args": [
        "-B",
        "build",
        "-G",
        "MinGW Makefiles"
      ]
    }
  ]
}
```

**사용법**: `Ctrl+Shift+B` (빌드 단축키)

---

## 💡 실전 예제: 계산기 라이브러리

### include/mylib/calculator.h

```c
#ifndef CALCULATOR_H
#define CALCULATOR_H

/**
 * @brief 두 정수를 더합니다
 * @param a 첫 번째 피연산자
 * @param b 두 번째 피연산자
 * @return 합계
 * 
 * @note 오버플로우 체크는 호출자의 책임입니다
 */
int add(int a, int b);

/**
 * @brief 두 정수를 나눕니다
 * @param a 피제수
 * @param b 제수
 * @param result 결과를 저장할 포인터 (NULL 가능)
 * @return 성공 시 0, 실패 시 -1
 * 
 * @warning b가 0이면 실패를 반환합니다
 */
int divide(int a, int b, int *result);

/**
 * @brief 배열의 평균을 계산합니다
 * @param arr 정수 배열
 * @param size 배열 크기
 * @return 평균값 (double)
 * 
 * @pre arr != NULL && size > 0
 */
double average(const int *arr, size_t size);

#endif /* CALCULATOR_H */
```

**설계 원칙**:
1. **헤더 가드**: 중복 포함 방지 (`#ifndef`)
2. **문서화**: 함수별 명확한 설명
3. **에러 처리**: 반환값으로 성공/실패 전달
4. **const 정확성**: 읽기 전용 포인터는 `const` 표시

### src/calculator.c

```c
#include "mylib/calculator.h"
#include <stddef.h>  /* size_t */

/* 왜 이렇게?: 간단한 함수는 인라인으로 최적화 가능성 */
int add(int a, int b) {
    return a + b;
}

/* 
 * 왜 이렇게?:
 * - 0으로 나누기 방지 (정의되지 않은 동작 예방)
 * - result가 NULL일 수 있으므로 체크 후 사용
 * - 반환값으로 성공/실패 전달 (POSIX 스타일)
 */
int divide(int a, int b, int *result) {
    if (b == 0) {
        return -1;  /* 실패 */
    }
    
    if (result != NULL) {
        *result = a / b;
    }
    
    return 0;  /* 성공 */
}

/*
 * 왜 이렇게?:
 * - const int *arr: 배열 수정 방지 (side effect 없음)
 * - size_t: 음수 크기는 의미 없으므로 unsigned 사용
 * - double 반환: 정수 나눗셈 손실 방지
 * - 방어적 프로그래밍: NULL 체크, 0 크기 체크
 */
double average(const int *arr, size_t size) {
    if (arr == NULL || size == 0) {
        return 0.0;
    }
    
    long long sum = 0;  /* 오버플로우 방지용 큰 타입 */
    
    for (size_t i = 0; i < size; i++) {
        sum += arr[i];
    }
    
    return (double)sum / size;  /* 명시적 캐스팅 */
}
```

**핵심 개념**:

| 패턴 | 이유 | 잘못된 예 |
|------|------|-----------|
| `const int *arr` | 함수가 배열을 수정하지 않음을 명시 | `int *arr` (수정 가능으로 오해) |
| `size_t` | 배열 크기는 항상 양수 | `int size` (음수 가능) |
| NULL 체크 | 포인터 역참조 전 항상 검증 | 바로 `*result = ...` |
| 에러 반환값 | 실패 가능한 함수는 상태 반환 | `void` 반환 (에러 처리 불가) |
| 명시적 캐스팅 | 타입 변환 의도 명확화 | `sum / size` (암시적 변환) |

### src/main.c

```c
#include <stdio.h>
#include "mylib/calculator.h"

int main(void) {
    /* 왜 void?: C에서 main()과 main(void)는 다름 */
    
    printf("Addition: %d\n", add(5, 3));
    
    int result;
    if (divide(10, 2, &result) == 0) {
        printf("Division: %d\n", result);
    } else {
        fprintf(stderr, "Error: Division by zero\n");
        /* 왜 stderr?: 에러 메시지는 표준 에러 스트림으로 */
    }
    
    int numbers[] = {1, 2, 3, 4, 5};
    size_t count = sizeof(numbers) / sizeof(numbers[0]);
    /* 왜 이렇게?: 배열 크기를 자동으로 계산 (매직 넘버 방지) */
    
    printf("Average: %.2f\n", average(numbers, count));
    
    return 0;
    /* 왜 0?: 0은 성공, 비-0은 에러 (Unix 규약) */
}
```

**하면 안 되는 것**:

```c
/* ❌ 나쁜 예 */
int main() {  /* void 없음 - 구식 스타일 */
    int arr[5] = {1,2,3,4,5};
    average(arr, 5);  /* 매직 넘버! */
    
    int x;
    x = divide(10, 0, &result);  /* 반환값 무시 */
    
    char *str = malloc(100);
    strcpy(str, "hello");  /* NULL 체크 안 함! */
    
    return 1;  /* 성공인데 에러 코드? */
}
```

---

## 🧪 테스트 코드 작성

### Unity 테스트 프레임워크 설정

```bash
# Unity 설치 (git submodule 사용)
git submodule add https://github.com/ThrowTheSwitch/Unity.git third_party/unity
```

### tests/CMakeLists.txt

```cmake
# Unity 프레임워크 추가
add_library(unity STATIC
    ${CMAKE_SOURCE_DIR}/third_party/unity/src/unity.c
)

target_include_directories(unity PUBLIC
    ${CMAKE_SOURCE_DIR}/third_party/unity/src
)

# 테스트 실행 파일
add_executable(test_calculator
    test_calculator.c
)

target_link_libraries(test_calculator
    calculator_lib
    unity
)

# CTest 통합
add_test(NAME CalculatorTests COMMAND test_calculator)
```

### tests/test_calculator.c

```c
#include "unity.h"
#include "mylib/calculator.h"

/* 
 * Unity 프레임워크 규칙:
 * - setUp(): 각 테스트 전 실행
 * - tearDown(): 각 테스트 후 실행
 * - test_로 시작하는 함수들이 테스트 케이스
 */

void setUp(void) {
    /* 테스트 전 초기화 */
}

void tearDown(void) {
    /* 테스트 후 정리 */
}

/* 기본 기능 테스트 */
void test_add_positive_numbers(void) {
    TEST_ASSERT_EQUAL_INT(8, add(5, 3));
    TEST_ASSERT_EQUAL_INT(0, add(0, 0));
}

void test_add_negative_numbers(void) {
    TEST_ASSERT_EQUAL_INT(-8, add(-5, -3));
    TEST_ASSERT_EQUAL_INT(2, add(5, -3));
}

/* 경계값 테스트 */
void test_divide_normal_case(void) {
    int result;
    int status = divide(10, 2, &result);
    
    TEST_ASSERT_EQUAL_INT(0, status);  /* 성공 확인 */
    TEST_ASSERT_EQUAL_INT(5, result);
}

void test_divide_by_zero(void) {
    int result;
    int status = divide(10, 0, &result);
    
    TEST_ASSERT_EQUAL_INT(-1, status);  /* 실패 확인 */
}

void test_divide_null_result(void) {
    /* result가 NULL이어도 크래시 안 나야 함 */
    int status = divide(10, 2, NULL);
    TEST_ASSERT_EQUAL_INT(0, status);
}

/* 배열 테스트 */
void test_average_normal_array(void) {
    int arr[] = {1, 2, 3, 4, 5};
    double result = average(arr, 5);
    
    TEST_ASSERT_DOUBLE_WITHIN(0.01, 3.0, result);
    /* 왜 WITHIN?: 부동소수점은 정확한 비교 불가 */
}

void test_average_null_array(void) {
    double result = average(NULL, 5);
    TEST_ASSERT_EQUAL_DOUBLE(0.0, result);
}

void test_average_zero_size(void) {
    int arr[] = {1, 2, 3};
    double result = average(arr, 0);
    TEST_ASSERT_EQUAL_DOUBLE(0.0, result);
}

/* Unity 실행 함수 */
int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_add_positive_numbers);
    RUN_TEST(test_add_negative_numbers);
    RUN_TEST(test_divide_normal_case);
    RUN_TEST(test_divide_by_zero);
    RUN_TEST(test_divide_null_result);
    RUN_TEST(test_average_normal_array);
    RUN_TEST(test_average_null_array);
    RUN_TEST(test_average_zero_size);
    
    return UNITY_END();
}
```

**테스트 전략**:

1. **정상 경로 (Happy Path)**: 기대대로 동작하는 경우
2. **경계값 (Boundary)**: 0, NULL, 최대/최소값
3. **에러 케이스 (Error)**: 잘못된 입력 처리
4. **엣지 케이스 (Edge)**: 특수한 조합

**실행**:
```bash
cmake --build build
ctest --test-dir build --output-on-failure
```

---

## 📚 고급 주제 학습 로드맵

### Week 1-2: 메모리 관리 마스터

**프로젝트**: 동적 배열 라이브러리 구현

```c
/* dynamic_array.h */
typedef struct {
    int *data;
    size_t size;
    size_t capacity;
} DynamicArray;

DynamicArray* da_create(size_t initial_capacity);
void da_push(DynamicArray *arr, int value);
void da_destroy(DynamicArray *arr);
```

**핵심 개념**:
- `malloc`, `realloc`, `free` 사용법
- 메모리 누수 방지
- Valgrind로 메모리 검사

### Week 3-4: 파일 I/O 및 데이터 구조

**프로젝트**: CSV 파서 및 해시 테이블

```c
/* csv_parser.h */
typedef struct {
    char **headers;
    char ***rows;
    size_t row_count;
} CSVData;

CSVData* parse_csv(const char *filename);
```

**핵심 개념**:
- `fopen`, `fread`, `fwrite`
- 버퍼링 및 에러 처리
- 링크드 리스트, 해시 테이블

### Week 5-6: 멀티스레딩 및 네트워크

**프로젝트**: 간단한 HTTP 서버

```c
/* server.h */
void start_server(int port);
void handle_request(int client_socket);
```

**핵심 개념**:
- POSIX threads (`pthread`)
- 소켓 프로그래밍
- 동기화 (mutex, semaphore)

---

## 🎯 일일 학습 루틴

### 1단계: 개념 학습 (30분)
- 새로운 C 개념 읽기
- 코드 예제 분석

### 2단계: 실습 (60분)
- 작은 기능 구현
- 테스트 코드 작성
- 여러 방식으로 빌드

### 3단계: 코드 리뷰 (30분)
- Valgrind로 메모리 체크
- 컴파일러 경고 0개 달성
- 코드 리팩토링

---

## 🔍 디버깅 도구

### GDB 기본 사용법

```bash
# 디버그 심볼 포함 빌드
gcc -g -o app main.c

# GDB 실행
gdb ./app

# 기본 명령어
(gdb) break main          # 브레이크포인트
(gdb) run                 # 실행
(gdb) next                # 다음 줄
(gdb) print variable      # 변수 출력
(gdb) backtrace           # 콜 스택
```

### Valgrind (Linux/macOS)

```bash
valgrind --leak-check=full ./app
```

---

## ✅ 체크리스트

매 프로젝트마다 확인:

- [ ] 헤더 가드 사용
- [ ] 모든 포인터 NULL 체크
- [ ] `const` 정확성 확보
- [ ] 메모리 누수 없음 (Valgrind 확인)
- [ ] 컴파일러 경고 0개 (`-Wall -Wextra`)
- [ ] 테스트 커버리지 80% 이상
- [ ] 크로스 플랫폼 빌드 성공
- [ ] 문서화 (Doxygen 스타일 주석)

---

## 🚀 다음 단계

이 가이드를 따라 학습하시면 3-4주 내에 실무 수준의 C 프로그래밍 능력을 갖추실 수 있습니다. 각 프로젝트를 완성하면서 자연스럽게 best practice를 체득하게 됩니다!

### 추천 학습 자료

- **책**: "The C Programming Language" (K&R), "Expert C Programming"
- **온라인**: CS50, Beej's Guide to C Programming
- **연습**: LeetCode (C 언어로 풀이), Project Euler
