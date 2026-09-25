add_library(tetris_sim INTERFACE)
target_include_directories(tetris_sim INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_features(tetris_sim INTERFACE cxx_std_23)
target_link_libraries(tetris_sim INTERFACE sgl_core)
