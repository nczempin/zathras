CC=g++
CFLAGS=-Ofast -g -std=c++17 -Wall -Wextra -pedantic -flto  -fno-builtin -m64 -malign-data=cacheline -march=sandybridge -I./zathras_lib/src #-fno-inline-small-functions #-fno-omit-frame-pointer

BIN=zathras
BUILD_DIR=build
SRC=$(wildcard src/*.cpp) $(wildcard zathras_lib/src/*.cpp)
CPP_OBJ=$(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SRC))

OBJ=$(CPP_OBJ)

all: $(BUILD_DIR) $(BIN)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)/src $(BUILD_DIR)/zathras_lib/src

$(BIN): $(OBJ)
	$(CC) -o $(BIN) $(CFLAGS)  $^

$(BUILD_DIR)/%.o: %.cpp
	$(CC) -o $@ -c $(CFLAGS) $<

.PHONY : clean
clean :
	-rm -f $(BIN)
	-rm -rf $(BUILD_DIR)
