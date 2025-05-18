CC=g++
CFLAGS=-Ofast -g -std=c++17 -Wall -Wextra -pedantic -flto -fno-builtin -m64 -malign-data=cacheline -march=sandybridge #-fno-inline-small-functions #-fno-omit-frame-pointer

BIN=zathras
# Recursively gather all source files from the engine and library
SRC := $(shell find src zathras_lib/src -name '*.cpp')
GAS=$(wildcard *.s)
NASM=$(wildcard *.asm)
CPP_OBJ=$(SRC:.cpp=.o)

OBJ=$(CPP_OBJ)
CPPFLAGS=-Izathras_lib/src

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) -o $(BIN) $(CFLAGS) $(OBJ)
%.o: %.cpp
	$(CC) -o $@ -c $(CFLAGS) $(CPPFLAGS) $<

.PHONY : clean
clean :
	-rm -f $(BIN) $(OBJ)
