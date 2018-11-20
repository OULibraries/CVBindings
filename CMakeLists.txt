cmake_minimum_required (VERSION 2.6)

project (CVBindings)

# Update binding installtion directories to match new opencv locations

set (CMAKE_CXX_FLAGS "-I/usr/local/include -I/usr/local/include/opencv -I/usr/local/include/opencv2")

add_library(CVBindings CVBindings.cpp)
