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

.PHONY : clean lint lint-clang-tidy lint-cppcheck
clean :
	-rm -f $(BIN) $(OBJ)

# Linting targets
lint: lint-clang-tidy lint-cppcheck

lint-clang-tidy:
	@echo "Running clang-tidy..."
	@if command -v clang-tidy >/dev/null 2>&1; then \
		find src zathras_lib/src -name '*.cpp' -o -name '*.h' | \
			xargs clang-tidy --config-file=.clang-tidy 2>/dev/null || true; \
	else \
		echo "clang-tidy not found. Please install: sudo apt-get install clang-tidy"; \
	fi

lint-cppcheck:
	@echo "Running cppcheck..."
	@cppcheck --enable=all --std=c++17 --inline-suppr \
		--suppress=missingIncludeSystem \
		-I./zathras_lib/src -I./src \
		src/*.cpp zathras_lib/src/*.cpp \
		--error-exitcode=0 2>&1 || true

# Run linting with error checking (for CI)
lint-ci: 
	@echo "Running clang-tidy (CI mode)..."
	@find src zathras_lib/src -name '*.cpp' -o -name '*.h' | \
		xargs clang-tidy --config-file=.clang-tidy --warnings-as-errors=* 2>/dev/null
	@echo "Running cppcheck (CI mode)..."
	@cppcheck --enable=all --std=c++17 --inline-suppr \
		--suppress=missingIncludeSystem \
		-I./zathras_lib/src -I./src \
		src/*.cpp zathras_lib/src/*.cpp \
		--error-exitcode=1
