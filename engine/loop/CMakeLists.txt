add_library(eng_loop STATIC
    src/FixedStepLoop.cpp
    src/App.cpp
)
target_include_directories(eng_loop PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_features(eng_loop PUBLIC cxx_std_23)
target_compile_options(eng_loop PRIVATE ${BASE_COMPILE_FLAGS})
target_link_libraries(eng_loop PUBLIC eng_scene eng_render)
