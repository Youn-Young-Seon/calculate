# Calculator Project

C 프로그래밍 학습을 위한 계산기 라이브러리 예제 프로젝트

## 프로젝트 구조

```
calculate/
├── include/mylib/      # 헤더 파일
│   └── calculator.h
├── src/                # 소스 파일
│   ├── calculator.c
│   └── main.c
├── tests/              # 테스트 코드
│   ├── test_calculator.c
│   └── CMakeLists.txt
├── build/              # 빌드 출력
├── CMakeLists.txt      # CMake 설정
├── Makefile            # Make 설정
└── README.md
```

## 빌드 방법

### 방법 1: CMake 사용 (권장)

```bash
# Windows
cmake -B build -G "MinGW Makefiles"
cmake --build build

# 실행
.\build\app.exe
```

### 방법 2: Makefile 사용

```bash
make          # 빌드
make test     # 실행
make clean    # 정리
```

### 방법 3: 직접 컴파일

```bash
gcc -Wall -Wextra -std=c11 -I./include -c src/calculator.c -o build/calculator.o
gcc -Wall -Wextra -std=c11 -I./include -c src/main.c -o build/main.o
gcc build/calculator.o build/main.o -o build/app.exe
```

## 테스트 실행

```bash
# CMake
cmake --build build
ctest --test-dir build --output-on-failure

# 또는 직접 실행
.\build\test_calculator.exe
```

## 기능

- `add()`: 두 정수 덧셈
- `divide()`: 안전한 정수 나눗셈 (0 체크 포함)
- `average()`: 배열의 평균 계산

## 학습 포인트

- 프로젝트 구조 설계
- 헤더 가드 사용
- 방어적 프로그래밍 (NULL 체크, 0 나누기 방지)
- const 정확성
- 테스트 주도 개발
- 크로스 플랫폼 빌드 시스템
