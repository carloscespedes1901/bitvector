cmake_minimum_required(VERSION 3.22)
project(bitvector)

set(CMAKE_CXX_STANDARD 17)

add_executable(bitvector main.cpp cmake-build-debug/pages.h)
