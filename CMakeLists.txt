cmake_minimum_required(VERSION 3.21.1)
project(Bitvector)

set(CMAKE_CXX_STANDARD 20)

include_directories(src)

add_subdirectory(src)
add_subdirectory(test)

