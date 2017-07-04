cmake_minimum_required(VERSION 3.4)
project(fuzzylogic)

set(CMAKE_BUILD_TYPE Release)
add_definitions(-std=c++14 -Ofast -march=native -pipe)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/QuercusOaxaca.txt ${CMAKE_CURRENT_BINARY_DIR}/QuercusOaxaca.txt COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/quercus_build_graph.sh ${CMAKE_CURRENT_BINARY_DIR}/quercus_build_graph.sh COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/lobatae.txt ${CMAKE_CURRENT_BINARY_DIR}/lobatae.txt COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/lobatae_build_graph.sh ${CMAKE_CURRENT_BINARY_DIR}/lobatae_build_graph.sh COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/redes_centro.txt ${CMAKE_CURRENT_BINARY_DIR}/redes_centro.txt COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/centro_build_graph.sh ${CMAKE_CURRENT_BINARY_DIR}/centro_build_graph.sh COPYONLY)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/analyze_graph.sage ${CMAKE_CURRENT_BINARY_DIR}/analyze_graph.sage COPYONLY)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake_modules")
message("Cmake modules in " ${CMAKE_MODULE_PATH})

find_package(Boost COMPONENTS program_options REQUIRED QUIET)
include_directories(${Boost_INCLUDE_DIRS})

OPTION(USE_FUZZY_MINIMUM_INSTEAD_OF_PRODUCT "Use the model of Fuzzy logic where the intersection is the minimum, not the product (warning: SLOW)" OFF)
OPTION(BUILD_USING_BLAZE "Use Blaze Library instead of Eigen (faster)" ON)
OPTION(BUILD_GPU "Build GPU version using arrayfire" ON)

if (${USE_FUZZY_MINIMUM_INSTEAD_OF_PRODUCT})
	set(BUILD_USING_BLAZE OFF)
	set(BUILD_GPU OFF)
	message("Blaze Library not available when using minimum instead of product")
endif()

if (BUILD_USING_BLAZE)
	find_package(BLAZE)
	if (${BLAZE_FOUND})
		include_directories(${BLAZE_INCLUDE_DIR})
		add_definitions(-DUSE_BLAZE)
		message("Blaze Library found at " ${BLAZE_INCLUDE_DIR})
	else()
		set(BUILD_USING_BLAZE OFF)
	endif()
else()
	message("Not using BLAZE! Trying to use eigen")
endif()
	
if (NOT ${BUILD_USING_BLAZE})
	find_package(Eigen3 REQUIRED QUIET)
	include_directories(${Eigen_INCLUDE_DIRS})
	message("Eigen Library found at " ${BLAZE_INCLUDE_DIR})
endif()

if (${USE_FUZZY_MINIMUM_INSTEAD_OF_PRODUCT})
	add_definitions(-DFUZZY_MIN)
endif()

set(EXTRA_LIBS ${Boost_LIBRARIES}  
			   ${CMAKE_THREAD_LIBS_INIT})

#################### Optional dependencies
OPTION(BUILD_USING_MP "Build Using OpenMP for parallelism" ON)
if (${BUILD_USING_MP})
	find_package(OpenMP QUIET)
	if (OPENMP_FOUND) 
		message("Open MP Found!")
		set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
		set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
		add_definitions(-DUSE_OPENMP)
	else()
		set(BUILD_USING_MP OFF)
	endif()
endif()


if (${BUILD_GPU})
	find_package(ArrayFire)
	find_package(CUDA) 
	find_package(NVVM)
	find_package(OpenCL)
	if(ArrayFire_CUDA_FOUND)
	include_directories(${ArrayFire_INCLUDE_DIRS})
		message("ArrayFire CUDA found at " ${ArrayFire_INCLUDE_DIRS})
		add_definitions(-DUSE_GPU)
		
		set(EXTRA_LIBS  ${EXTRA_LIBS} 
					    ${ArrayFire_CUDA_LIBRARIES} 
					    ${CUDA_LIBRARIES} 
					    ${NVVM_LIB})
					   
	elseif(ArrayFire_OpenCL_FOUND)
		include_directories(${ArrayFire_INCLUDE_DIRS})
		add_definitions(-DUSE_GPU)
		
		set(EXTRA_LIBS  ${EXTRA_LIBS} 
						${ArrayFire_OpenCL_LIBRARIES} 
						${OpenCL_LIBRARIES})
						
		message("ArrayFire OpenCL found at "  ${ArrayFire_INCLUDE_DIRS})
	else()
		message("ArrayFire not found! To use the GPU, you need to install ArrayFire. Falling back to CPU.")
		set(BUILD_GPU OFF)
	endif()
endif()


message("Extra libs: " ${EXTRA_LIBS})

set(MYSOURCES main.cpp Point.cpp ReadFile.cpp argumentparser.cpp GraphCalculator.cpp)

add_executable(fuzzylogic ${MYSOURCES})

target_link_libraries(fuzzylogic ${EXTRA_LIBS})
