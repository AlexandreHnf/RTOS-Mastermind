cmake_minimum_required(VERSION 3.14)
project(RTOS-Mastermind)

set(CMAKE_CXX_STANDARD 17)

add_executable(RTOS-Mastermind main.cpp Game.cpp Game.h GameMaster.cpp GameMaster.h Player.cpp Player.h)