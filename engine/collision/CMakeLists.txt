add_library(eng_collision STATIC
    src/Collision.cpp
)
target_include_directories(eng_collision PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_features(eng_collision PUBLIC cxx_std_23)
target_compile_options(eng_collision PRIVATE ${BASE_COMPILE_FLAGS})
target_link_libraries(eng_collision PUBLIC eng_core)
