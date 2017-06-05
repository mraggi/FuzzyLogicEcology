cmake_minimum_required(VERSION 2.6)
project(fuzzylogic)

set(CMAKE_BUILD_TYPE Release)
add_definitions(-std=c++14)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/QuercusOaxaca.txt ${CMAKE_CURRENT_BINARY_DIR}/QuercusOaxaca.txt COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/lobatae.txt ${CMAKE_CURRENT_BINARY_DIR}/lobatae.txt COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/redes_centro.txt ${CMAKE_CURRENT_BINARY_DIR}/redes_centro.txt COPYONLY)

add_subdirectory(Graph)
include_directories(Graph)


find_package(ArrayFire)
include_directories(${ArrayFire_INCLUDE_DIRS})
find_package(CUDA) 
find_package(NVVM)
set(EXTRA_LIBS ${CMAKE_THREAD_LIBS_INIT} ${CUDA_LIBRARIES} ${NVVM_LIB})

if (ArrayFire_CUDA_FOUND)
	add_definitions(-DUSE_GPU)
elseif(ArrayFire_OpenCL_FOUND)
	add_definitions(-DUSE_GPU)
endif()



add_executable(fuzzylogic main.cpp Point.cpp ReadFile.cpp Mu.cpp GraphCalculator.cpp)
target_link_libraries(fuzzylogic graph ${ArrayFire_LIBRARIES} ${EXTRA_LIBS})
