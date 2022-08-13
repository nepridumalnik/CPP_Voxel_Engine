set(TARGET Engine)

set(EXECUTABLE_SOURCES
    # main
    main.cpp
)

set(LIBRARY_SOURCES
    # engine
    # window
    window/Window.cpp
    window/Events.cpp
    window/Camera.cpp

    # graphics
    graphics/Shader.cpp
    graphics/Texture.cpp
    graphics/Mesh.cpp
    graphics/VoxelRenderer.cpp

    # voxels
    voxels/voxel.cpp
    voxels/Chunk.cpp
    voxels/Chunks.cpp

    # utils
    utils/loaders.cpp
)
set(PRECOMPILE_HEADERS
    ${CMAKE_SOURCE_DIR}/src/utils/macros.hpp
    ${CMAKE_SOURCE_DIR}/pch/OpenGL.hpp
    ${CMAKE_SOURCE_DIR}/pch/GLM.hpp
)
set(LIBRARIES
    glfw
    GLEW::GLEW
    PNG::PNG
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(${TARGET} SHARED ${LIBRARY_SOURCES})
target_precompile_headers(${TARGET} PRIVATE ${PRECOMPILE_HEADERS})
target_link_libraries(${TARGET} ${LIBRARIES})

add_executable(main ${EXECUTABLE_SOURCES})
target_precompile_headers(main REUSE_FROM ${TARGET})
target_link_libraries(main ${TARGET} ${LIBRARIES})
