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
