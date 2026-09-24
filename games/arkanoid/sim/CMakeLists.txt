add_library(arkanoid_sim INTERFACE)
target_include_directories(arkanoid_sim INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_features(arkanoid_sim INTERFACE cxx_std_23)
target_link_libraries(arkanoid_sim INTERFACE eng_collision)
