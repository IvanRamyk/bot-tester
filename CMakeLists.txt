cmake_minimum_required(VERSION 3.15)
project(test_project)

set(CMAKE_CXX_STANDARD 14)


add_executable(tester main.cpp game/Quoridor.cpp game/Quoridor.hpp tests/quoridor_tests.cpp server/Server.cpp
        server/Server.hpp)

add_definitions(-DBOOST_ERROR_CODE_HEADER_ONLY)

target_link_libraries(tester
        ${Boost_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
        )
target_include_directories(tester
        PRIVATE
        ${Boost_INCLUDE_DIR}
        )


target_link_libraries(tester gtest gtest_main)

