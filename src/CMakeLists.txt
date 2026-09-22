add_library(gameLib
    STATIC
        Ball.cpp
        Brick.cpp
        Game.cpp
        GameObject.cpp
        GameplayScreen.cpp
        InputMapper.cpp
        MainMenuScreen.cpp
        Paddle.cpp
        PauseScreen.cpp
        ScreenStack.cpp
        World.cpp
        makeWorld.cpp
)
target_compile_features(gameLib PRIVATE cxx_std_23)
target_compile_options(gameLib PRIVATE ${BASE_COMPILE_FLAGS})
target_link_libraries(gameLib PUBLIC SFML::Graphics SFML::System SFML::Window)

add_executable(game main.cpp draw.cpp)
target_compile_features(game PRIVATE cxx_std_23)
target_compile_options(game PRIVATE ${BASE_COMPILE_FLAGS})
target_link_libraries(game PRIVATE gameLib)
