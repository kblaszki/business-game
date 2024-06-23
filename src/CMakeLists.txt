add_library(gameLib
    SHARED
        controllers/EventController.cpp
        controllers/GameController.cpp
        
        core/WindowSFML.cpp

        managers/ClosedEventManager.cpp
        managers/KeyPressedManager.cpp
        managers/KeyReleasedManager.cpp

        utils/EventPrinter.cpp
)
target_compile_features(gameLib PRIVATE cxx_std_20)
target_compile_options(gameLib PRIVATE ${BASE_COMPILE_FLAGS})
target_link_libraries(gameLib PUBLIC sfml-graphics sfml-system sfml-window)

add_executable(game main.cpp)
target_compile_features(game PRIVATE cxx_std_20)
target_compile_options(game PRIVATE ${BASE_COMPILE_FLAGS})
target_link_libraries(game PRIVATE gameLib)