cmake_minimum_required(VERSION 3.15)
project(tester)

set(CMAKE_CXX_STANDARD 14)
add_subdirectory(lib/googletest-master)
include_directories(lib/googletest-master/googletest/include)
include_directories(lib/googletest-master/googlemock/include)

add_executable(tester main.cpp Quoridor.cpp Quoridor.hpp tests/quoridor_tests.cpp)
target_link_libraries(tester gtest gtest_main)