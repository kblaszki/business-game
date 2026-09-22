add_library(gameLib
    STATIC
        controllers/EventController.cpp
        controllers/GameController.cpp
        controllers/ScreenController.cpp

        entities/Ball.cpp
        entities/Brick.cpp
        entities/Button.cpp
        entities/Paddle.cpp

        handlers/OnClickHandler.cpp
        handlers/OnHoverHandler.cpp

        managers/ManagerOf.cpp
        managers/GameExitManager.cpp
        managers/GameWindowManager.cpp
        managers/KeyboardManager.cpp
        managers/MouseManager.cpp

        resources/ResourceManager.cpp

        screens/GameScreen.cpp
        screens/MenuScreen.cpp

        window/WindowSFML.cpp
)
target_compile_features(gameLib PRIVATE cxx_std_23)
target_compile_options(gameLib PRIVATE ${BASE_COMPILE_FLAGS})
target_link_libraries(gameLib PUBLIC SFML::Graphics SFML::System SFML::Window)

add_executable(game main.cpp)
target_compile_features(game PRIVATE cxx_std_23)
target_compile_options(game PRIVATE ${BASE_COMPILE_FLAGS})
target_link_libraries(game PRIVATE gameLib)

set(RESOURCES_DIRECTORY "${CMAKE_SOURCE_DIR}/resources")
set(RESOURCES_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources")

add_custom_command(
    TARGET game PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${RESOURCES_DIRECTORY} ${RESOURCES_OUTPUT_DIRECTORY}
    COMMENT "Copying the resources directory to the build directory"
)
