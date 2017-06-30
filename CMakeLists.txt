cmake_minimum_required(VERSION 3.4)
project(fuzzylogic)

set(CMAKE_BUILD_TYPE Release)
add_definitions(-std=c++14 -O3 -march=native)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/QuercusOaxaca.txt ${CMAKE_CURRENT_BINARY_DIR}/QuercusOaxaca.txt COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/quercus_build_graph.sh ${CMAKE_CURRENT_BINARY_DIR}/quercus_build_graph.sh COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/lobatae.txt ${CMAKE_CURRENT_BINARY_DIR}/lobatae.txt COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/lobatae_build_graph.sh ${CMAKE_CURRENT_BINARY_DIR}/lobatae_build_graph.sh COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/redes_centro.txt ${CMAKE_CURRENT_BINARY_DIR}/redes_centro.txt COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/centro_build_graph.sh ${CMAKE_CURRENT_BINARY_DIR}/centro_build_graph.sh COPYONLY)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/analyze_graph.sage ${CMAKE_CURRENT_BINARY_DIR}/analyze_graph.sage COPYONLY)

# add_subdirectory(Graph)
# include_directories(Graph)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake_modules")
message("Cmake modules in " ${CMAKE_MODULE_PATH})

find_package(Boost COMPONENTS program_options REQUIRED)
find_package(BLAZE REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${BLAZE_INCLUDE_DIR})
set(EXTRA_LIBS ${Boost_LIBRARIES}  ${CMAKE_THREAD_LIBS_INIT}) 
message("Blaze Library found at " ${BLAZE_INCLUDE_DIR})


#################### Optional dependencies
find_package(ArrayFire)
find_package(CUDA) 
find_package(NVVM)
find_package(OpenCL)

find_package(OpenMP)
if (OPENMP_FOUND) 
	message("Open MP Found!")
	set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
	add_definitions(-DUSE_OPENMP)
endif()

if(ArrayFire_CUDA_FOUND)
	include_directories(${ArrayFire_INCLUDE_DIRS})
	message("ArrayFire CUDA found at " ${ArrayFire_INCLUDE_DIRS})
	add_definitions(-DUSE_GPU)
	set(EXTRA_LIBS ${EXTRA_LIBS} ${ArrayFire_CUDA_LIBRARIES} ${CUDA_LIBRARIES} ${NVVM_LIB})
elseif(ArrayFire_OpenCL_FOUND)
	include_directories(${ArrayFire_INCLUDE_DIRS})
	add_definitions(-DUSE_GPU)
	set(EXTRA_LIBS ${EXTRA_LIBS} ${ArrayFire_OpenCL_LIBRARIES} ${OpenCL_LIBRARIES})
	message("ArrayFire OpenCL found at "  ${ArrayFire_INCLUDE_DIRS})
else()
	message("ArrayFire not found! You won't be using your GPU. Don't worry, it's probably fast enough as it is with only CPU.")
endif()


message("Extra libs: " ${EXTRA_LIBS})

set(MYSOURCES main.cpp Point.cpp ReadFile.cpp argumentparser.cpp GraphCalculator_GPU.cpp)

add_executable(fuzzylogic ${MYSOURCES})

target_link_libraries(fuzzylogic ${EXTRA_LIBS})
