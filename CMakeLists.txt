cmake_minimum_required(VERSION 3.4)
project(fuzzylogic)

set(CMAKE_BUILD_TYPE Release)
add_definitions(-std=c++14 -O2 -march=native -pipe)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/dummytest.txt ${CMAKE_CURRENT_BINARY_DIR}/dummytest.txt COPYONLY)
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

OPTION(BUILD_TESTS ON)

OPTION(USE_FUZZY_MINIMUM_INSTEAD_OF_PRODUCT "Use the model of Fuzzy logic where the intersection is the minimum, not the product (warning: SLOW)" OFF)

OPTION(BUILD_USING_BLAZE "Use Blaze Library for Linear Algebra" OFF)
OPTION(BUILD_USING_EIGEN "Use Eigen Library for Linear Algebra" ON)

if (${USE_FUZZY_MINIMUM_INSTEAD_OF_PRODUCT})
    set(BUILD_USING_EIGEN ON)
	set(BUILD_USING_BLAZE OFF)
	message("Blaze Library not available when using minimum instead of product")
endif()

if (BUILD_USING_BLAZE)
    set(BUILD_USING_EIGEN OFF)
	find_package(BLAZE)
    include_directories(${BLAZE_INCLUDE_DIR})
    add_definitions(-DUSE_BLAZE)
    message("Using Blaze Library. Blaze Library found at " ${BLAZE_INCLUDE_DIR})
else()
	set(BUILD_USING_EIGEN ON)
endif()

if (BUILD_USING_EIGEN)
    set(BUILD_USING_BLAZE  OFF)
	find_package(Eigen3 REQUIRED QUIET)
    include_directories(${EIGEN_INCLUDE_DIR})
    add_definitions(-DUSE_EIGEN)
    message("Using Eigen Library  (ver " ${EIGEN3_VERSION} ").")
endif()

if (BUILD_USING_BLAZE AND BUILD_USING_EIGEN)
    message("FATAL ERROR: You must either choose BUILD_USING_BLAZE or BUILD_USING_EIGEN!")
endif()

if (${USE_FUZZY_MINIMUM_INSTEAD_OF_PRODUCT})
	add_definitions(-DFUZZY_MIN)
endif()

# add_definitions(-DPROMISCUIDAD)

set(EXTRA_LIBS ${Boost_LIBRARIES}  
			   ${CMAKE_THREAD_LIBS_INIT})

#################### Optional dependencies
OPTION(BUILD_USING_MP "Build Using OpenMP for parallelism" ON)
if (${BUILD_USING_MP})
#     set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp=libomp")
#     set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp=libomp")
#     include_directories("/usr/lib/gcc/x86_64-pc-linux-gnu/6.3.1/include/")
	find_package(OpenMP)
	if (OPENMP_FOUND) 
		message("Open MP Found! Code will use multiple cores!")
		set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
		set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
		add_definitions(-DUSE_OPENMP)
	else()
		set(BUILD_USING_MP OFF)
		message("OpenMP not found! Turning off openmp :(")
	endif()
endif()


message("Extra libs: " ${EXTRA_LIBS})

set(MYSOURCES main.cpp Point.cpp ReadFile.cpp argumentparser.cpp GraphCalculator.cpp GraphConnectedComponents.cpp)

add_executable(fuzzylogic ${MYSOURCES})

target_link_libraries(fuzzylogic ${EXTRA_LIBS})

if (${BUILD_TESTS})
	add_executable(utests UTests.cpp Point.cpp GraphCalculator.cpp)
	target_link_libraries(utests ${EXTRA_LIBS})
endif()
