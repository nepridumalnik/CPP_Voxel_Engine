set(TARGET RESOURCES)

set(DIRECTORIES
    textures
    shaders
)

add_custom_target(${TARGET} ALL
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/textures
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

foreach(directory ${DIRECTORIES})
    add_custom_command(TARGET ${TARGET} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/${directory} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${directory}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endforeach()
