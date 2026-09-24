add_library(arkanoid_app INTERFACE)
target_include_directories(arkanoid_app INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_features(arkanoid_app INTERFACE cxx_std_23)
target_link_libraries(arkanoid_app INTERFACE arkanoid_sim eng_loop eng_resources)
