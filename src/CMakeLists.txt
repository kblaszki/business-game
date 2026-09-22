add_library(gameLib
    STATIC
        Example.cpp
)
target_compile_features(gameLib PRIVATE cxx_std_23)
target_compile_options(gameLib PRIVATE ${BASE_COMPILE_FLAGS})

add_executable(game main.cpp)
target_compile_features(game PRIVATE cxx_std_23)
target_compile_options(game PRIVATE ${BASE_COMPILE_FLAGS})
target_link_libraries(game PRIVATE gameLib SFML::Graphics SFML::System SFML::Window)
