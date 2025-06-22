CC=g++
CFLAGS=-Ofast -g -std=c++17 -Wall -Wextra -pedantic -flto  -fno-builtin -m64 -malign-data=cacheline -march=sandybridge -I./zathras_lib/src #-fno-inline-small-functions #-fno-omit-frame-pointer

BIN= zathras
SRC=$(wildcard src/*.cpp)
LIB_SRC=$(wildcard zathras_lib/src/*.cpp)
LIB_OBJ=$(LIB_SRC:.cpp=.o)
GAS=$(wildcard *.s)
NASM=$(wildcard *.asm)
CPP_OBJ=$(SRC:.cpp=.o)

OBJ=$(CPP_OBJ) $(LIB_OBJ)

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) -o $(BIN) $(CFLAGS)  $^
%.o: %.cpp
	$(CC) -o $@ -c $(CFLAGS) $^

.PHONY : clean
clean :
	-rm -f $(BIN) $(OBJ)
