CC=g++
CFLAGS=-Ofast -march=sandybridge -mtune=sandybridge -mno-avx -mno-aes -std=c++11 -Wall -Wextra -pedantic

BIN= zathras
SRC=$(wildcard **/*.cpp)
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
