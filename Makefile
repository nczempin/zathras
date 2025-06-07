CC=g++
# Include paths needed for headers under src/ and zathras_lib/src
INCLUDES=-Isrc -Izathras_lib/src
CFLAGS=-Ofast -g -std=c++17 -Wall -Wextra -pedantic -flto  -fno-builtin -m64 -malign-data=cacheline -march=sandybridge $(INCLUDES) #-fno-inline-small-functions #-fno-omit-frame-pointer
CPPFLAGS=-Izathras_lib/src -Itests/external

BIN= zathras
LIB= libzathras.a
TEST_LIB= libzathras_test.a
# Recursively gather all C++ sources under the project
SRC=$(shell find src zathras_lib/src -name '*.cpp')
GAS=$(wildcard *.s)
NASM=$(wildcard *.asm)
CPP_OBJ=$(SRC:.cpp=.o)

LIB_SRCS := $(wildcard zathras_lib/src/*.cpp)
LIB_OBJS := $(LIB_SRCS:.cpp=.o)
TEST_LIB_OBJS := $(LIB_SRCS:.cpp=.test.o)

APP_SRCS := $(wildcard src/*.cpp)
APP_OBJS := $(APP_SRCS:.cpp=.o)

# Gather all test sources from the tests directory tree
TEST_SRCS := $(shell find tests -name '*.cpp')
TEST_OBJS := $(TEST_SRCS:.cpp=.o)
TEST_BIN := run_tests

OBJ=$(LIB_OBJS) $(APP_OBJS)

all: $(BIN)

$(LIB): $(LIB_OBJS)
	ar rcs $@ $^

$(TEST_LIB): $(TEST_LIB_OBJS)
	ar rcs $@ $^

$(BIN): $(APP_OBJS) $(LIB)
	$(CC) $(CFLAGS) -o $@ $(APP_OBJS) $(LIB)

$(TEST_BIN): $(TEST_OBJS) $(TEST_LIB)
	$(CC) $(CFLAGS) -fno-lto -o $@ $(TEST_OBJS) $(TEST_LIB)

%.o: %.cpp
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

tests/%.o: tests/%.cpp
	$(CC) $(CPPFLAGS) $(CFLAGS) -fno-lto -c $< -o $@

%.test.o: %.cpp
	$(CC) $(CPPFLAGS) $(CFLAGS) -fno-lto -c $< -o $@

simple_test: tests/simple_test.o $(TEST_LIB)
	$(CC) $(CFLAGS) -fno-lto -o $@ $< $(TEST_LIB)

bitboard_test: tests/bitboard_test.o $(TEST_LIB)
	$(CC) $(CFLAGS) -fno-lto -o $@ $< $(TEST_LIB)

test: simple_test bitboard_test
	./simple_test
	./bitboard_test

.PHONY: clean
clean:
	rm -f $(BIN) $(OBJ) $(LIB) $(TEST_LIB) $(TEST_LIB_OBJS) $(TEST_BIN) $(TEST_OBJS) simple_test bitboard_test
