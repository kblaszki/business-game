add_library(eng_render INTERFACE)
target_include_directories(eng_render INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_features(eng_render INTERFACE cxx_std_23)
target_link_libraries(eng_render INTERFACE eng_core)
