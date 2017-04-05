cmake_minimum_required(VERSION 2.6)
project(fuzzylogic)
add_definitions(-std=c++14)

add_executable(fuzzylogic main.cpp Point.cpp)

install(TARGETS fuzzylogic RUNTIME DESTINATION bin)
