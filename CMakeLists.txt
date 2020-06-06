cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
project(tester)

find_package(Boost COMPONENTS system REQUIRED)

set(CMAKE_CXX_STANDARD 14)
add_subdirectory(lib/googletest-master)
include_directories(lib/googletest-master/googletest/include)
include_directories(lib/googletest-master/googlemock/include)

add_executable(tester main.cpp game/Quoridor.cpp game/Quoridor.hpp tests/quoridor_tests.cpp server/Server.cpp
        server/Server.hpp tournament/tournament.cpp game/TestGame.cpp game/TestGame.h tournament/tournament.h)

add_definitions(-DBOOST_ERROR_CODE_HEADER_ONLY)
include_directories(PRIVATE ${Boost_INCLUDE_DIR})
link_directories(LINK_PUBLIC ${Boost_LIBRARY_DIR})

target_link_libraries(tester
        ${Boost_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
        )
target_include_directories(tester
        PRIVATE
        ${Boost_INCLUDE_DIR}
        )


target_link_libraries(tester gtest gtest_main)