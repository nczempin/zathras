CC=g++
# Include paths needed for headers under src/ and zathras_lib/src
INCLUDES=-Isrc -Izathras_lib/src
CFLAGS=-Ofast -g -std=c++17 -Wall -Wextra -pedantic -flto  -fno-builtin -m64 -malign-data=cacheline -march=sandybridge $(INCLUDES) #-fno-inline-small-functions #-fno-omit-frame-pointer
CPPFLAGS=-Izathras_lib/src -Itests/external

BIN= zathras
LIB= libzathras.a
# Recursively gather all C++ sources under the project
SRC=$(shell find src zathras_lib/src -name '*.cpp')
GAS=$(wildcard *.s)
NASM=$(wildcard *.asm)
CPP_OBJ=$(SRC:.cpp=.o)

LIB_SRCS := $(wildcard zathras_lib/src/*.cpp)
LIB_OBJS := $(LIB_SRCS:.cpp=.o)

APP_SRCS := $(wildcard src/*.cpp)
APP_OBJS := $(APP_SRCS:.cpp=.o)

OBJ=$(LIB_OBJS) $(APP_OBJS)

TEST_SRCS := $(wildcard tests/unit/*.cpp)
TEST_OBJS := $(TEST_SRCS:.cpp=.o)
TEST_BIN := run_tests

all: $(BIN)

$(LIB): $(LIB_OBJS)
	ar rcs $@ $^

$(BIN): $(APP_OBJS) $(LIB)
	$(CC) $(CFLAGS) -o $@ $(APP_OBJS) $(LIB)

%.o: %.cpp
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(TEST_BIN): $(TEST_OBJS) $(LIB)
	$(CC) $(CFLAGS) -o $@ $(TEST_OBJS) $(LIB)

test: $(TEST_BIN)
	./$(TEST_BIN)

.PHONY: clean
clean:
	rm -f $(BIN) $(OBJ) $(LIB)
