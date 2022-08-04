find_package(glfw3 CONFIG REQUIRED)
find_package(glew CONFIG REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(pch ${GLEW_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS})

add_compile_options(-Winvalid-pch)

include(CheckIPOSupported)
check_ipo_supported(RESULT supported OUTPUT error)

if(supported)
    message(STATUS "IPO/LTO enabled")
    set(PROPERTY INTERPROCEDURAL_OPTIMIZATION ON)
else()
    message(STATUS "IPO/LTO not supported: <${error}>")
endif()
