add_library(sgl_fx INTERFACE)
target_include_directories(sgl_fx INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_features(sgl_fx INTERFACE cxx_std_23)
target_link_libraries(sgl_fx INTERFACE sgl_core sgl_render)
