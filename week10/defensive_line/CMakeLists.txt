cmake_minimum_required(VERSION 3.1)
project(defensive_line)

set(CMAKE_VERBOSE_MAKEFILE on)

find_package(CGAL REQUIRED COMPONENTS Core)
find_package(Boost REQUIRED)
include_directories(${CGAL_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
include( ${CGAL_USE_FILE} )

FILE(GLOB_RECURSE sources src/*.cpp)
add_executable(${PROJECT_NAME} ${sources})

target_link_libraries(${PROJECT_NAME} ${CGAL_LIBRARIES} ${Boost_LIBRARIES} ${MPFR_LIBRARIES} ${GMP_LIBRARIES} ${THREAD_LIBRARIES})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED on)

target_compile_options(${PROJECT_NAME} PRIVATE -O2 -Wall -Wextra -pedantic -Wfloat-equal -Wconversion -Wlogical-op -Wshift-overflow=2 -Wduplicated-cond)
