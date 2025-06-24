CC=g++
CFLAGS=-Ofast -g -std=c++17 -Wall -Wextra -pedantic -flto  -fno-builtin -m64 -malign-data=cacheline -march=sandybridge -I./zathras_lib/src #-fno-inline-small-functions #-fno-omit-frame-pointer

BIN= zathras
SRC=$(wildcard src/*.cpp) $(wildcard zathras_lib/src/*.cpp)
GAS=$(wildcard *.s)
NASM=$(wildcard *.asm)
CPP_OBJ=$(SRC:.cpp=.o)

OBJ=$(CPP_OBJ)

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) -o $(BIN) $(CFLAGS)  $^
%.o: %.cpp
	$(CC) -o $@ -c $(CFLAGS) $^

.PHONY : clean
clean :
	-rm -f $(BIN) $(OBJ)
