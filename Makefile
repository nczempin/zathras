CC=g++
# Include paths needed for headers under src/ and zathras_lib/src
INCLUDES=-Isrc -Izathras_lib/src
CFLAGS=-Ofast -g -std=c++17 -Wall -Wextra -pedantic -flto  -fno-builtin -m64 -malign-data=cacheline -march=sandybridge $(INCLUDES) #-fno-inline-small-functions #-fno-omit-frame-pointer

BIN= zathras
# Recursively gather all C++ sources under the project
SRC=$(shell find . -name '*.cpp')
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
