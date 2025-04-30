cmake_minimum_required(VERSION 3.31)
project(Benchmark)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -DNDEBUG")

add_executable(BE be.cpp)
add_executable(LE le.cpp)
