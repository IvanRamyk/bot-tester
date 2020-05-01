cmake_minimum_required(VERSION 3.15)
project(tester)

set(CMAKE_CXX_STANDARD 14)

add_executable(tester main.cpp Quoridor.cpp Quoridor.hpp)