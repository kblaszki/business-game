add_library(eng_sfml STATIC
    src/SfmlModule.cpp
)
target_include_directories(eng_sfml PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_features(eng_sfml PUBLIC cxx_std_23)
target_compile_options(eng_sfml PRIVATE ${BASE_COMPILE_FLAGS})
target_link_libraries(eng_sfml
    PUBLIC
        eng_loop
        eng_resources
    PRIVATE
        SFML::Graphics
        SFML::Window
        SFML::System
)
