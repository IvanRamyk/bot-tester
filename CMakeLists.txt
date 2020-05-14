cmake_minimum_required(VERSION 3.15)
project(test_project)

set(CMAKE_CXX_STANDARD 14)

add_executable(tester main.cpp server/Server.hpp server/Server.cpp
        game/Quoridor.cpp game/Quoridor.hpp
        Interactor/Interactor.h Interactor/Interactor.cpp
        tournament/tournament.cpp)