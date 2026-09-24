add_library(eng_scene INTERFACE)
target_include_directories(eng_scene INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_features(eng_scene INTERFACE cxx_std_23)
target_link_libraries(eng_scene INTERFACE eng_input)
