cmake_minimum_required(VERSION 2.6)
project(fuzzylogic)

set(CMAKE_BUILD_TYPE Release)
add_definitions(-std=c++14)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/QuercusOaxaca.txt ${CMAKE_CURRENT_BINARY_DIR}/QuercusOaxaca.txt COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/lobatae.txt ${CMAKE_CURRENT_BINARY_DIR}/lobatae.txt COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/redes_centro.txt ${CMAKE_CURRENT_BINARY_DIR}/redes_centro.txt COPYONLY)

add_subdirectory(Graph)
include_directories(Graph)

add_executable(fuzzylogic main.cpp Point.cpp ReadFile.cpp Mu.cpp GraphCalculator.cpp)
target_link_libraries(fuzzylogic graph)