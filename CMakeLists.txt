cmake_minimum_required(VERSION 3.27)
project(tinyprotocol C)

set(CMAKE_C_STANDARD 17)

add_library(tinyprotocol STATIC library.c
        config.h)

# Tests
add_executable(test_local_buffer tests/local_buffer.c)
target_link_libraries(test_local_buffer tinyprotocol)