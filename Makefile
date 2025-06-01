CC=g++
CFLAGS=-Ofast -g -std=c++17 -Wall -Wextra -pedantic -flto -fno-builtin -m64 -malign-data=cacheline -march=sandybridge -pthread
CPPFLAGS=-Izathras_lib/src -Itests/external

BIN=zathras
LIB=libzathras.a

LIB_SRCS := $(wildcard zathras_lib/src/*.cpp)
LIB_OBJS := $(LIB_SRCS:.cpp=.o)

TEST_SRCS := $(wildcard tests/*.cpp)
TEST_OBJS := $(TEST_SRCS:.cpp=.o)
TEST_BIN := tests/test_runner

APP_SRCS := $(wildcard src/*.cpp)
APP_OBJS := $(APP_SRCS:.cpp=.o)

OBJ=$(LIB_OBJS) $(APP_OBJS)

all: $(BIN)

test: $(TEST_BIN)
	./$(TEST_BIN)

$(TEST_BIN): $(LIB) $(TEST_OBJS)
	$(CC) $(CFLAGS) -o $@ $(TEST_OBJS) $(LIB)

$(LIB): $(LIB_OBJS)
	ar rcs $@ $^

$(BIN): $(APP_OBJS) $(LIB)
	$(CC) $(CFLAGS) -o $@ $(APP_OBJS) $(LIB)

%.o: %.cpp
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(BIN) $(OBJ) $(LIB) $(TEST_BIN) $(TEST_OBJS)
