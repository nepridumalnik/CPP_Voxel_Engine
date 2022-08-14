cmake_minimum_required(VERSION 3.20)
project(opengl)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/modules")
include(config)
include(headers)

add_subdirectory(src)
add_subdirectory(resources)
