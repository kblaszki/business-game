add_library(sgl_collision STATIC
    src/Collision.cpp
)
target_include_directories(sgl_collision PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_features(sgl_collision PUBLIC cxx_std_23)
target_compile_options(sgl_collision PRIVATE ${BASE_COMPILE_FLAGS})
target_link_libraries(sgl_collision PUBLIC sgl_core)
