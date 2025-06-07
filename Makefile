CC=g++
CXXFLAGS=-Ofast -g -std=c++17 -Wall -Wextra -pedantic -flto -fno-builtin -m64 -malign-data=cacheline -march=sandybridge
INCLUDES=-Izathras_lib/src -Isrc -Itests/external

BIN=zathras

LIB_SRC=$(filter-out zathras_lib/src/Bitboard_test.cpp, $(wildcard zathras_lib/src/*.cpp))
APP_SRC=$(wildcard src/*.cpp)
SRC=$(APP_SRC) $(LIB_SRC)
OBJ=$(SRC:.cpp=.o)

TEST_SRC=$(wildcard tests/*.cpp)
TEST_OBJ=$(TEST_SRC:.cpp=.o)
TEST_BIN=tests/test

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CXXFLAGS) $(INCLUDES) -o $@ $^

%.o: %.cpp
	$(CC) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

tests/%.o: tests/%.cpp
	$(CC) $(filter-out -flto,$(CXXFLAGS)) $(INCLUDES) -c $< -o $@

# Create non-LTO versions of library objects for tests
TEST_LIB_OBJ=$(LIB_SRC:.cpp=_test.o)
TEST_APP_OBJ=$(filter-out src/main_test.o,$(APP_SRC:.cpp=_test.o))

%_test.o: %.cpp
	$(CC) $(filter-out -flto,$(CXXFLAGS)) $(INCLUDES) -c $< -o $@

$(TEST_BIN): $(TEST_OBJ) $(TEST_LIB_OBJ) $(TEST_APP_OBJ)
	$(CC) $(filter-out -flto,$(CXXFLAGS)) $(INCLUDES) -o $@ $^

.PHONY: clean test

test: $(TEST_BIN)
	./$(TEST_BIN)

clean:
	-rm -f $(BIN) $(OBJ) $(TEST_OBJ) $(TEST_LIB_OBJ) $(TEST_APP_OBJ) $(TEST_BIN)
