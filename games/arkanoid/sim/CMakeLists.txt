add_library(arkanoid_sim STATIC
    src/Effects.cpp
    src/Levels.cpp
    src/Physics.cpp
)
target_include_directories(arkanoid_sim PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_features(arkanoid_sim PUBLIC cxx_std_23)
target_compile_options(arkanoid_sim PRIVATE ${BASE_COMPILE_FLAGS})
target_link_libraries(arkanoid_sim PUBLIC sgl_collision)
