set(TARGET main)

set(SOURCES
    main.cpp

    # subsystems
    # window
    window/Window.cpp
    window/Events.cpp

    # graphics
    graphics/Shader.cpp
)
set(PRECOMPILE_HEADERS
    ${CMAKE_SOURCE_DIR}/pch/OpenGL.hpp
    # ${CMAKE_SOURCE_DIR}/pch/GLM.hpp
)
set(LIBRARIES
    glfw
    GLEW::GLEW
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${TARGET} ${SOURCES})
target_precompile_headers(${TARGET} PRIVATE ${PRECOMPILE_HEADERS})
target_link_libraries(${TARGET} ${LIBRARIES})
