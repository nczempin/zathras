CC=g++
CFLAGS=-Ofast -g -std=c++17 -Wall -Wextra -pedantic -flto -fno-builtin -m64 -malign-data=cacheline -march=sandybridge -pthread
CPPFLAGS=-Izathras_lib/src

BIN=zathras
LIB=libzathras.a

LIB_SRCS := $(wildcard zathras_lib/src/*.cpp)
LIB_OBJS := $(LIB_SRCS:.cpp=.o)

APP_SRCS := $(wildcard src/*.cpp)
APP_OBJS := $(APP_SRCS:.cpp=.o)

OBJ=$(LIB_OBJS) $(APP_OBJS)

all: $(BIN)

$(LIB): $(LIB_OBJS)
	ar rcs $@ $^

$(BIN): $(APP_OBJS) $(LIB)
	$(CC) $(CFLAGS) -o $@ $(APP_OBJS) $(LIB)

%.o: %.cpp
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(BIN) $(OBJ) $(LIB)
