add_library(eng_collision INTERFACE)
target_include_directories(eng_collision INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_features(eng_collision INTERFACE cxx_std_23)
target_link_libraries(eng_collision INTERFACE eng_core)
