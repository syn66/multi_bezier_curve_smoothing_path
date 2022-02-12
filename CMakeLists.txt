cmake_minimum_required(VERSION 3.7)
project(test_bezier)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-O2")

include_directories(.)
include_directories(matplotlibcpp)


add_executable(test_bezier
test_bezier.cpp
                src/bezier.h
                src/bezier.cpp
                )


find_package(Python COMPONENTS Development)
target_include_directories(test_bezier PRIVATE ${Python_INCLUDE_DIRS})
target_link_libraries(test_bezier ${Python_LIBRARIES})