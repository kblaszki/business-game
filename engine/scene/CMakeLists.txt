add_library(sgl_scene STATIC
    src/SceneContext.cpp
    src/SceneStack.cpp
)
target_include_directories(sgl_scene PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_features(sgl_scene PUBLIC cxx_std_23)
target_compile_options(sgl_scene PRIVATE ${BASE_COMPILE_FLAGS})
target_link_libraries(sgl_scene PUBLIC sgl_input)
