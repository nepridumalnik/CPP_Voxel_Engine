include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/src/engine)

add_executable(voxels voxels.cpp)
target_add_pch_glew(voxels)
target_add_pch_definitions(voxels)
target_link_libraries(voxels Voxels Engine glfw GLEW::GLEW)
