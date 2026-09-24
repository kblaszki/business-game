add_library(eng_render STATIC
    src/RenderQueue.cpp
)
target_include_directories(eng_render PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_features(eng_render PUBLIC cxx_std_23)
target_compile_options(eng_render PRIVATE ${BASE_COMPILE_FLAGS})
target_link_libraries(eng_render PUBLIC eng_core)
