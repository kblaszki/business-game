add_library(gameLib
    STATIC
        Game.cpp
        InputMapper.cpp
        World.cpp
        makeWorld.cpp
        entities/Ball.cpp
        entities/Brick.cpp
        entities/Button.cpp
        entities/Paddle.cpp
        screens/GameplayScreen.cpp
        screens/MainMenuScreen.cpp
        screens/PauseScreen.cpp
        screens/ScreenStack.cpp
        window/SfmlDrawer.cpp
        window/WindowSFML.cpp
)
target_compile_features(gameLib PRIVATE cxx_std_23)
target_compile_options(gameLib PRIVATE ${BASE_COMPILE_FLAGS})
target_link_libraries(gameLib PUBLIC SFML::Graphics SFML::System SFML::Window)

add_executable(game main.cpp)
target_compile_features(game PRIVATE cxx_std_23)
target_compile_options(game PRIVATE ${BASE_COMPILE_FLAGS})
target_link_libraries(game PRIVATE gameLib)
