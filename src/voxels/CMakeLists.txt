set(TARGET Voxels)

set(SOURCES
    # voxels
    voxel.cpp
    Chunk.cpp
    Chunks.cpp
    VoxelRenderer.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(${TARGET} SHARED ${SOURCES})
target_add_pch_glew(${TARGET})
target_add_pch_definitions(${TARGET})
target_link_libraries(${TARGET} PRIVATE Engine)
