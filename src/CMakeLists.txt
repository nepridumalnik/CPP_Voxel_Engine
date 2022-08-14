set(TARGET Main)

set(SOURCES
    # main
    main.cpp
)
set(LIBRARIES
    # project
    Engine
    Voxels

    # 3rd party
    glfw
    GLEW::GLEW
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/engine)

add_subdirectory(engine)
add_subdirectory(voxels)

add_executable(${TARGET} ${SOURCES})
target_add_pch_glew(${TARGET})
target_add_pch_definitions(${TARGET})
target_link_libraries(${TARGET} ${LIBRARIES})
