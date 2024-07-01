add_library(gameLib
    SHARED
        controllers/EventController.cpp
        controllers/GameController.cpp
        controllers/ScreenController.cpp

        entities/Button.cpp
        entities/Player.cpp

        handlers/OnHoverHandler.cpp

        managers/ManagerOf.cpp
        managers/GameExitManager.cpp
        managers/KeyboardManager.cpp
        managers/MouseManager.cpp

        screens/MenuScreen.cpp
        
        utils/EventPrinter.cpp
        
        window/WindowSFML.cpp
)
target_compile_features(gameLib PRIVATE cxx_std_20)
target_compile_options(gameLib PRIVATE ${BASE_COMPILE_FLAGS})
target_link_libraries(gameLib PUBLIC sfml-graphics sfml-system sfml-window)

add_executable(game main.cpp)
target_compile_features(game PRIVATE cxx_std_20)
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
