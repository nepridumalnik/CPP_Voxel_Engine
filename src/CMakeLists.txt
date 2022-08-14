set(TARGET Engine)

set(EXECUTABLE_SOURCES
    # main
    main.cpp
)

set(ENGINE_SOURCES
    # engine
    # window
    window/Window.cpp
    window/Events.cpp
    window/Camera.cpp

    # graphics
    graphics/Shader.cpp
    graphics/Texture.cpp
    graphics/Mesh.cpp

    # utils
    utils/loaders.cpp
)
set(VOXELS_SOURCES
    # voxels
    voxels/voxel.cpp
    voxels/Chunk.cpp
    voxels/Chunks.cpp
    voxels/VoxelRenderer.cpp
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

add_library(${TARGET} SHARED ${ENGINE_SOURCES})
target_precompile_headers(${TARGET} PRIVATE ${PRECOMPILE_HEADERS})
target_link_libraries(${TARGET} PUBLIC ${LIBRARIES})

add_library(Voxels SHARED ${VOXELS_SOURCES})
target_precompile_headers(Voxels REUSE_FROM ${TARGET})
target_link_libraries(Voxels PUBLIC ${TARGET})

add_executable(main ${EXECUTABLE_SOURCES})
target_precompile_headers(main REUSE_FROM ${TARGET})
target_link_libraries(main ${TARGET} ${LIBRARIES} Voxels)
