# zathras

I wanted to get more seriously into more modern C++, bitboards, and eventually a multithreaded chess engine.

So I got started on Zathras, named after my favourite character from the Babylon 5 universe.

I only got as far as using modern C++ async I/O capabilities, but making it truly multithreaded would eventually have made the "Zathras" name very fitting.

## Running tests

The project uses [Catch2](https://github.com/catchorg/Catch2) for unit testing.
The single-header distribution is already included under
`tests/external/catch_amalgamated.hpp`, so no additional downloads are
required.

To build and run tests with a C++17 compiler, compile your test sources and
include the `tests` directory so that the Catch2 header can be found. For
example:

```bash
g++ -std=c++17 -Isrc -Itests -o my_tests path/to/my_tests.cpp
./my_tests
```
