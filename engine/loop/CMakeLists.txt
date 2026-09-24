add_library(eng_loop INTERFACE)
target_include_directories(eng_loop INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_features(eng_loop INTERFACE cxx_std_23)
target_link_libraries(eng_loop INTERFACE eng_scene eng_render)
