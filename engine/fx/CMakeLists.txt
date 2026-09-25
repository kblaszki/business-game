add_library(sgl_fx STATIC
    src/ParticleSystem.cpp
)
target_include_directories(sgl_fx PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_features(sgl_fx PUBLIC cxx_std_23)
target_compile_options(sgl_fx PRIVATE ${BASE_COMPILE_FLAGS})
target_link_libraries(sgl_fx PUBLIC sgl_core sgl_render)
