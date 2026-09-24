add_library(eng_scene STATIC
    src/SceneContext.cpp
    src/SceneStack.cpp
)
target_include_directories(eng_scene PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_features(eng_scene PUBLIC cxx_std_23)
target_compile_options(eng_scene PRIVATE ${BASE_COMPILE_FLAGS})
target_link_libraries(eng_scene PUBLIC eng_input)
