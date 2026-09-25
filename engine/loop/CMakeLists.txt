add_library(sgl_loop STATIC
    src/FixedStepLoop.cpp
    src/App.cpp
)
target_include_directories(sgl_loop PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_features(sgl_loop PUBLIC cxx_std_23)
target_compile_options(sgl_loop PRIVATE ${BASE_COMPILE_FLAGS})
target_link_libraries(sgl_loop PUBLIC sgl_scene sgl_render)
