cmake_minimum_required(VERSION 3.15)
project(test_project)

set(CMAKE_CXX_STANDARD 14)

add_executable(tester main.cpp client/Client.h client/Client.cpp game/Quoridor.cpp game/Quoridor.hpp Interactor/Interactor.h Interactor/Interactor.cpp client/Client.cpp client/Client.h tournament/tournament.cpp)