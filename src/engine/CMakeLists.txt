set(TARGET Engine)

set(SOURCES
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
set(LIBRARIES
    # OpenGL
    glfw
    GLEW::GLEW

    # image loader
    PNG::PNG
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(${TARGET} SHARED ${SOURCES})
target_add_pch_glew(${TARGET})
target_add_pch_definitions(${TARGET})
target_link_libraries(${TARGET} PRIVATE ${LIBRARIES})
