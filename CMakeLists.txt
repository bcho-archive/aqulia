cmake_minimum_required (VERSION 2.8)
project (aqulia)

set (CMAKE_INSTALL_PREFIX "./bin/")
set (CMAKE_C_FLAGS "-Wall -g")

add_subdirectory (src)
add_subdirectory (tests)
