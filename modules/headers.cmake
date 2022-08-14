set(GLEW_HEADERS
    ${CMAKE_SOURCE_DIR}/pch/OpenGL.hpp
    ${CMAKE_SOURCE_DIR}/pch/GLM.hpp
)

set(DEFINITION_HEADERS
    ${CMAKE_SOURCE_DIR}/src/engine/utils/macros.hpp
)

function(target_add_pch_glew target)
    target_precompile_headers(${target} PRIVATE ${GLEW_HEADERS})
endfunction()

function(target_add_pch_definitions target)
    target_precompile_headers(${target} PRIVATE ${DEFINITION_HEADERS})
endfunction()
