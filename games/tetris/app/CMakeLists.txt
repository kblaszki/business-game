add_library(tetris_app INTERFACE)
target_include_directories(tetris_app INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_features(tetris_app INTERFACE cxx_std_23)
target_link_libraries(tetris_app INTERFACE
    tetris_sim
    sgl_loop
    sgl_resources
    sgl_audio
    sgl_fx
    sgl_save
)
