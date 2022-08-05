set(TARGET TEXTURES)

file(GLOB IMAGES *.png *.bmp *.jpg)

add_custom_target(${TARGET} ALL
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/textures
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

foreach(texture ${IMAGES})
    file(RELATIVE_PATH file ${CMAKE_CURRENT_SOURCE_DIR} ${texture})

    add_custom_command(TARGET ${TARGET} POST_BUILD

        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${file} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/textures/${file}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endforeach()
