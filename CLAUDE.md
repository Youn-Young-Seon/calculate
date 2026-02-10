# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is a C programming learning project implementing a calculator library. It demonstrates best practices for C11 development on Windows with cross-platform compatibility in mind.

## Environment Setup

- **Platform**: Windows (primary), with cross-platform support
- **Compiler**: GCC 15.2.0 via MinGW-w64 (MSYS2)
- **C Standard**: C11
- **Path to compiler**: `C:\msys64\mingw64\bin\gcc.exe`

When running GCC commands, always prefix with:
```bash
export PATH="/c/msys64/mingw64/bin:$PATH"
```

## Build Commands

### Direct Compilation (Recommended for this project)
```bash
# Compile library
gcc -Wall -Wextra -std=c11 -I./include -c src/calculator.c -o build/calculator.o

# Compile main
gcc -Wall -Wextra -std=c11 -I./include -c src/main.c -o build/main.o

# Link
gcc build/calculator.o build/main.o -o build/app.exe

# Run
./build/app.exe
```

### Compile and Run Tests
```bash
gcc -Wall -Wextra -std=c11 -I./include tests/test_calculator.c src/calculator.c -o build/test_calculator.exe
./build/test_calculator.exe
```

### Using Makefile (Note: Currently has issues on Windows)
```bash
mingw32-make           # Build
mingw32-make clean     # Clean build artifacts
```

### Using CMake
```bash
cmake -B build -G "MinGW Makefiles"
cmake --build build
ctest --test-dir build --output-on-failure
```

## Architecture

### Module Structure

The project follows a library-based architecture with clear separation of concerns:

1. **Public API** (`include/mylib/calculator.h`):
   - Must include `<stddef.h>` for `size_t` type
   - Uses header guards to prevent multiple inclusion
   - All functions documented with Doxygen-style comments

2. **Implementation** (`src/calculator.c`):
   - Defensive programming: NULL checks, division by zero protection
   - Uses `const` correctness for read-only parameters
   - Returns status codes (0 = success, -1 = error) for operations that can fail

3. **Test Suite** (`tests/test_calculator.c`):
   - Simple macro-based test framework (no external dependencies)
   - Tests cover: normal cases, boundary conditions, error cases, NULL handling

### Key Design Patterns

- **Error Handling**: Functions that can fail return status codes; output via pointer parameters
- **Type Safety**: Use `size_t` for array sizes (never negative), explicit casts for type conversions
- **Memory Safety**: All pointer parameters validated before dereferencing

### Compilation Requirements

- **Compiler Flags**: Always use `-Wall -Wextra -std=c11` to catch potential issues
- **Include Path**: `-I./include` required for header files
- **Output Directory**: All build artifacts go in `build/` directory

## Windows-Specific Considerations

- Use `.exe` extension for executables
- Windows bash environment interprets paths as Unix-style (`/c/Users/...`)
- Use `mingw32-make` instead of `make` command
- Ensure GCC is in PATH or use absolute path: `/c/msys64/mingw64/bin/gcc.exe`

## Common Patterns When Modifying Code

1. When adding new functions to the calculator library:
   - Add declaration to `include/mylib/calculator.h` with documentation
   - Add implementation to `src/calculator.c`
   - Add test cases to `tests/test_calculator.c`
   - Recompile both main app and test suite

2. When fixing compilation errors:
   - Check that `<stddef.h>` is included if using `size_t`
   - Verify include paths are correct (`-I./include`)
   - Ensure C11 standard is specified (`-std=c11`)

3. Test execution always returns exit code 0 for success, 1 for failure
