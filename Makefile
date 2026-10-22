CC=g++
CFLAGS=-Ofast -g -std=c++17 -Wall -Wextra -pedantic -flto -fno-builtin -m64 -malign-data=cacheline -march=sandybridge -I./zathras_lib/src

BIN=zathras
TEST_BIN=zathras_tests
BUILD_DIR=build
LIB_SRC=$(wildcard zathras_lib/src/*.cpp)
SRC=$(wildcard src/*.cpp) $(LIB_SRC)
CPP_OBJ=$(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SRC))
LIB_OBJ=$(patsubst %.cpp,$(BUILD_DIR)/%.o,$(LIB_SRC))

OBJ=$(CPP_OBJ)

all: $(BUILD_DIR) $(BIN)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)/src $(BUILD_DIR)/zathras_lib/src $(BUILD_DIR)/tests

$(BIN): $(OBJ)
	$(CC) -o $(BIN) $(CFLAGS) $^

$(BUILD_DIR)/%.o: %.cpp
	$(CC) -o $@ -c $(CFLAGS) $<

test: $(BUILD_DIR) $(LIB_OBJ) $(BUILD_DIR)/tests/test_main.o
	$(CC) -o $(TEST_BIN) $(CFLAGS) $(LIB_OBJ) $(BUILD_DIR)/tests/test_main.o
	./$(TEST_BIN)

lint-cppcheck:
	cppcheck --enable=warning,performance,portability --std=c++17 -I./zathras_lib/src src/ zathras_lib/src/

.PHONY : all clean test lint-cppcheck
clean :
	-rm -f $(BIN) $(TEST_BIN)
	-rm -rf $(BUILD_DIR)
