# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is a C/C++ programming learning project implementing multiple utility libraries: calculator, dynamic arrays, CSV parsing, and TCP networking.

- **C version**: Demonstrates C11 best practices with manual memory management
- **C++ version**: Demonstrates modern C++17 with OOP, RAII, STL containers, and exceptions

Both versions provide the same functionality but use different programming paradigms.

## Environment Setup

- **Platform**: Windows (primary), with cross-platform support
- **Compilers**:
  - C: GCC 15.2.0 via MinGW-w64 (MSYS2)
  - C++: G++ 15.2.0 via MinGW-w64 (MSYS2)
- **Standards**:
  - C11 for C code
  - C++17 for C++ code
- **Path to compilers**:
  - `C:\msys64\mingw64\bin\gcc.exe`
  - `C:\msys64\mingw64\bin\g++.exe`

When running GCC/G++ commands, always prefix with:
```bash
export PATH="/c/msys64/mingw64/bin:$PATH"
```

## Build Commands

### Direct Compilation (for quick single-module builds)

#### C Version
```bash
# Example: Building main app with calculator module only
gcc -Wall -Wextra -std=c11 -I./include -c src/calculator.c -o build/calculator.o
gcc -Wall -Wextra -std=c11 -I./include -c src/main.c -o build/main.o
gcc build/calculator.o build/main.o -o build/app.exe
./build/app.exe

# Example: Building TCP server (note: requires -lws2_32 on Windows)
gcc -Wall -Wextra -std=c11 -I./include -c src/tcp_server.c -o build/tcp_server.o
gcc -Wall -Wextra -std=c11 -I./include -c src/server_main.c -o build/server_main.o
gcc build/tcp_server.o build/server_main.o -o build/tcp_server.exe -lws2_32
./build/tcp_server.exe
```

#### C++ Version
```bash
# Example: Building main app with Calculator module
g++ -Wall -Wextra -std=c++17 -I./include -c src/cpp/Calculator.cpp -o build/Calculator.o
g++ -Wall -Wextra -std=c++17 -I./include -c src/cpp/DynamicArray.cpp -o build/DynamicArray.o
g++ -Wall -Wextra -std=c++17 -I./include -c src/cpp/CSVParser.cpp -o build/CSVParser.o
g++ -Wall -Wextra -std=c++17 -I./include -c src/cpp/main.cpp -o build/main_cpp.o
g++ build/Calculator.o build/DynamicArray.o build/CSVParser.o build/main_cpp.o -o build/app_cpp.exe
./build/app_cpp.exe

# Example: Building TCP server (note: requires -lws2_32 on Windows)
g++ -Wall -Wextra -std=c++17 -I./include -c src/cpp/TCPServer.cpp -o build/TCPServer.o
g++ -Wall -Wextra -std=c++17 -I./include -c src/cpp/server_main.cpp -o build/server_main_cpp.o
g++ build/TCPServer.o build/server_main_cpp.o -o build/tcp_server_cpp.exe -lws2_32
./build/tcp_server_cpp.exe
```

### Compile and Run Tests

#### C Tests
```bash
# Calculator tests
gcc -Wall -Wextra -std=c11 -I./include tests/test_calculator.c src/calculator.c -o build/test_calculator.exe
./build/test_calculator.exe

# Dynamic array tests
gcc -Wall -Wextra -std=c11 -I./include tests/test_dynamic_array.c src/dynamic_array.c -o build/test_dynamic_array.exe
./build/test_dynamic_array.exe

# CSV parser tests
gcc -Wall -Wextra -std=c11 -I./include tests/test_csv_parser.c src/csv_parser.c -o build/test_csv_parser.exe
./build/test_csv_parser.exe

# TCP client test (requires running server first)
gcc -Wall -Wextra -std=c11 -I./include tests/test_tcp_client.c src/tcp_server.c -o build/test_tcp_client.exe -lws2_32
./build/test_tcp_client.exe
```

#### C++ Tests
```bash
# Calculator C++ tests
g++ -Wall -Wextra -std=c++17 -I./include tests/cpp/test_calculator.cpp src/cpp/Calculator.cpp -o build/test_calculator_cpp.exe
./build/test_calculator_cpp.exe

# DynamicArray C++ tests
g++ -Wall -Wextra -std=c++17 -I./include tests/cpp/test_dynamic_array.cpp src/cpp/DynamicArray.cpp -o build/test_dynamic_array_cpp.exe
./build/test_dynamic_array_cpp.exe

# CSVParser C++ tests
g++ -Wall -Wextra -std=c++17 -I./include tests/cpp/test_csv_parser.cpp src/cpp/CSVParser.cpp -o build/test_csv_parser_cpp.exe
./build/test_csv_parser_cpp.exe

# TCP Client C++ test (requires running server first)
g++ -Wall -Wextra -std=c++17 -I./include tests/cpp/test_tcp_client.cpp src/cpp/TCPServer.cpp -o build/test_tcp_client_cpp.exe -lws2_32
./build/test_tcp_client_cpp.exe
```

### Using Makefile (Note: Currently has issues on Windows)
```bash
mingw32-make           # Build
mingw32-make clean     # Clean build artifacts
```

### Using CMake (Recommended for full project)
CMake automatically handles all dependencies, library linking, and platform-specific flags for both C and C++.

```bash
# Configure and build
cmake -B build -G "MinGW Makefiles"
cmake --build build

# Run all tests
ctest --test-dir build --output-on-failure

# C Build outputs:
# - build/libcalculator_lib.a (C static library)
# - build/app.exe (C main application)
# - build/tcp_server.exe (C TCP echo server)
# - build/tests/test_*.exe (C test executables)

# C++ Build outputs:
# - build/libcalculator_lib_cpp.a (C++ static library)
# - build/app_cpp.exe (C++ main application)
# - build/tcp_server_cpp.exe (C++ TCP echo server)
# - build/tests/test_*_cpp.exe (C++ test executables)
```

## Architecture

### Project Structure

```
calculate/
├── include/
│   ├── mylib/          # C headers
│   │   ├── calculator.h
│   │   ├── dynamic_array.h
│   │   ├── csv_parser.h
│   │   └── tcp_server.h
│   └── cpp/mylib/      # C++ headers
│       ├── Calculator.hpp
│       ├── DynamicArray.hpp
│       ├── CSVParser.hpp
│       └── TCPServer.hpp
├── src/                # C implementations
├── src/cpp/            # C++ implementations
├── tests/              # C tests
└── tests/cpp/          # C++ tests
```

### Module Comparison: C vs C++

#### 1. Calculator Module
**C Version** (`calculator.h/c`):
- Function-based API
- Returns status codes (0=success, -1=error)
- Output via pointer parameters
- Manual error checking required

**C++ Version** (`Calculator.hpp/cpp`):
- Static class methods (namespace-like organization)
- Throws exceptions for errors (`std::invalid_argument`)
- Uses `std::vector` for array operations
- Exception-based error handling

#### 2. Dynamic Array Module
**C Version** (`dynamic_array.h/c`):
- Manual memory management with `malloc`/`realloc`
- Requires `da_create()` and `da_destroy()`
- Returns status codes
- NULL pointer checks throughout

**C++ Version** (`DynamicArray.hpp/cpp`):
- RAII pattern - automatic memory management
- Wraps `std::vector<int>`
- Constructor/destructor handles resources
- Operator overloading (`[]` for array access)
- Exception-based bounds checking

#### 3. CSV Parser Module
**C Version** (`csv_parser.h/c`):
- Manual string parsing with C string functions
- Dynamic memory for 2D string arrays
- Requires `csv_destroy()` for cleanup
- Returns status codes

**C++ Version** (`CSVParser.hpp/cpp`):
- Uses `std::string` and `std::vector`
- Automatic memory management
- RAII - destructor handles cleanup
- `std::ifstream` for file I/O
- Exception-based error handling

#### 4. TCP Server Module
**C Version** (`tcp_server.h/c`):
- Structure-based with function pointers
- Manual socket lifecycle management
- Explicit cleanup with `tcp_server_shutdown()`
- Returns status codes

**C++ Version** (`TCPServer.hpp/cpp`):
- Class-based design with private members
- RAII - destructor closes sockets automatically
- Move semantics (no copy allowed for sockets)
- Exception-based error reporting
- Separate `TCPClient` utility class

### Design Patterns

#### C Design Patterns
1. **Public API** (headers in `include/mylib/`):
   - Include `<stddef.h>` for `size_t` type
   - Header guards to prevent multiple inclusion
   - Doxygen-style documentation

2. **Implementation** (files in `src/`):
   - Defensive programming: NULL checks, bounds checking
   - `const` correctness for read-only parameters
   - Status codes: 0 = success, -1 = error
   - Manual resource management

3. **Test Suites** (files in `tests/`):
   - Macro-based test framework
   - Cover: normal cases, boundary conditions, error cases, NULL handling

#### C++ Design Patterns
1. **Public API** (headers in `include/cpp/mylib/`):
   - `.hpp` extension for C++ headers
   - Header guards
   - `namespace mylib` for all classes
   - Doxygen-style documentation

2. **Implementation** (files in `src/cpp/`):
   - RAII pattern for all resource management
   - Exception-based error handling (`std::exception` hierarchy)
   - STL containers (`std::vector`, `std::string`)
   - Rule of Five for classes managing resources
   - `const` correctness and modern C++ best practices

3. **Test Suites** (files in `tests/cpp/`):
   - Exception testing with try-catch blocks
   - `assert()` for validation
   - Cover: normal cases, exceptions, move semantics

### Key Design Patterns

- **Error Handling**: Functions that can fail return status codes; output via pointer parameters
- **Type Safety**: Use `size_t` for array sizes (never negative), explicit casts for type conversions
- **Memory Safety**: All pointer parameters validated before dereferencing

### Compilation Requirements

#### C Compilation
- **Compiler**: GCC with flags `-Wall -Wextra -std=c11`
- **Include Path**: `-I./include`
- **Output Directory**: `build/`
- **TCP Module**: Requires `-lws2_32` on Windows
- **Static Library**: `libcalculator_lib.a`

#### C++ Compilation
- **Compiler**: G++ with flags `-Wall -Wextra -std=c++17`
- **Include Path**: `-I./include`
- **Output Directory**: `build/`
- **TCP Module**: Requires `-lws2_32` on Windows
- **Static Library**: `libcalculator_lib_cpp.a`
- **Exception Support**: Enabled by default in G++

## Windows-Specific Considerations

- Use `.exe` extension for executables
- Windows bash environment interprets paths as Unix-style (`/c/Users/...`)
- Use `mingw32-make` instead of `make` command
- Ensure GCC is in PATH or use absolute path: `/c/msys64/mingw64/bin/gcc.exe`

## Common Patterns When Modifying Code

### C Code Modifications

1. **Adding new functions**:
   - Add declaration to header in `include/mylib/` with Doxygen comments
   - Add implementation to corresponding `.c` file in `src/`
   - Add test cases to corresponding test file in `tests/`
   - Update CMakeLists.txt if adding new source files
   - Recompile and run tests

2. **Working with memory-managed structures**:
   - Always check return value from create functions (NULL = failure)
   - Always call corresponding destroy function when done
   - Never access freed memory
   - Watch for memory leaks with valgrind (Linux) or similar tools

3. **Error handling**:
   - Functions return 0 for success, -1 for error
   - Check return values before using output parameters
   - Provide detailed error messages via stderr

### C++ Code Modifications

1. **Adding new methods**:
   - Add declaration to header in `include/cpp/mylib/` with Doxygen comments
   - Add implementation to corresponding `.cpp` file in `src/cpp/`
   - Add test cases to corresponding test file in `tests/cpp/`
   - Update CMakeLists.txt if adding new source files
   - Recompile and run tests

2. **Working with RAII classes**:
   - Resources are automatically managed by constructors/destructors
   - No manual cleanup needed (no `delete` or `free`)
   - Use move semantics for resource transfer
   - Avoid copying resource-owning objects (use `= delete` for copy operations)

3. **Error handling**:
   - Throw appropriate exceptions (`std::invalid_argument`, `std::runtime_error`, `std::out_of_range`)
   - Use try-catch blocks in tests to verify exception behavior
   - Let exceptions propagate to caller for library code

### Common to Both

1. **Working with TCP modules**:
   - Always link with `-lws2_32` on Windows
   - Test with client after making server changes
   - Server must be running before client tests

2. **Fixing compilation errors**:
   - C: Check `<stddef.h>` for `size_t`, use `-std=c11`
   - C++: Use `-std=c++17`, check namespace usage
   - Verify include paths: `-I./include`
   - For TCP: ensure Winsock is linked on Windows

3. **Test execution**: All tests return exit code 0 for success, 1 for failure

### Learning Points

- Compare C and C++ implementations side-by-side to understand:
  - Manual vs automatic memory management
  - Status codes vs exceptions
  - Procedural vs object-oriented design
  - C strings vs `std::string`
  - Manual arrays vs STL containers
