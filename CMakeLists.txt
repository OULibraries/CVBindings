cmake_minimum_required (VERSION 2.6)

project (CVBindings)

set (CMAKE_CXX_FLAGS "-I/usr/local/opt/opencv3/include -I/usr/local/opt/opencv3/include/opencv -I/usr/local/opt/opencv3/include/opencv2")

add_library(CVBindings CVBindings.cpp)