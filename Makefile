CC=g++
CFLAGS=-Ofast -g -std=c++17 -Wall -Wextra -pedantic -flto  -fno-builtin -m64 -malign-data=cacheline -march=sandybridge -I./zathras_lib/src #-fno-inline-small-functions #-fno-omit-frame-pointer

BIN= zathras
SRC=$(wildcard src/*.cpp)
LIB_OBJ=$(wildcard zathras_lib/src/*.o)
GAS=$(wildcard *.s)
NASM=$(wildcard *.asm)
CPP_OBJ=$(SRC:.cpp=.o)

OBJ=$(CPP_OBJ) $(LIB_OBJ)

TEST_SRCS := $(wildcard tests/unit/*.cpp)
TEST_OBJS := $(TEST_SRCS:.cpp=.o)
TEST_BIN := run_tests

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) -o $(BIN) $(CFLAGS)  $^
%.o: %.cpp
	$(CC) -o $@ -c $(CFLAGS) $(CPPFLAGS) $^

$(TEST_BIN): $(TEST_OBJS) $(LIB)
	$(CC) $(CFLAGS) -o $@ $(TEST_OBJS) $(LIB)

test: $(TEST_BIN)
	./$(TEST_BIN)

.PHONY: clean
clean:
	rm -f $(BIN) $(OBJ) $(LIB)
