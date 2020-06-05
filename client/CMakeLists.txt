set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
cmake_minimum_required(VERSION 3.5)
project(testClient)

set(Boost_INCLUDE_DIR /usr/local/src/boost_1_73_0)
set(Boost_LIBRARY_DIR /usr/local/src/boost_1_73_0/stage/lib)
find_package(Boost 1.73 COMPONENTS system REQUIRED)
include_directories(PRIVATE ${Boost_INCLUDE_DIR})
link_directories(LINK_PUBLIC ${Boost_LIBRARY_DIR})

set(CMAKE_CXX_STANDARD 14)

add_executable(testClient main.cpp ../server/Server.cpp ../server/Server.hpp)

target_link_libraries(testClient ${Boost_LIBRARIES} )
