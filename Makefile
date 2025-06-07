CC=g++
CXXFLAGS=-Ofast -g -std=c++17 -Wall -Wextra -pedantic -flto -fno-builtin -m64 -malign-data=cacheline -march=sandybridge
INCLUDES=-Izathras_lib/src -Isrc -Itests/external

BIN=zathras

SRC=$(wildcard src/*.cpp) $(filter-out zathras_lib/src/Bitboard_test.cpp, $(wildcard zathras_lib/src/*.cpp))
OBJ=$(SRC:.cpp=.o)

TEST_SRC=$(wildcard tests/*.cpp) $(wildcard zathras_lib/src/*_test.cpp)
TEST_OBJ=$(TEST_SRC:.cpp=.o)
TEST_BIN=tests/test

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CXXFLAGS) $(INCLUDES) -o $@ $^

%.o: %.cpp
	$(CC) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(TEST_BIN): $(TEST_OBJ) $(filter-out src/main.o,$(OBJ))
	$(CC) $(CXXFLAGS) $(INCLUDES) -o $@ $^

.PHONY: clean test

test: $(TEST_BIN)
	./$(TEST_BIN)

clean:
	-rm -f $(BIN) $(OBJ) $(TEST_OBJ) $(TEST_BIN)
