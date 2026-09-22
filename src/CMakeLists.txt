add_library(gameLib
    STATIC
        Example.cpp
        Game.cpp
        screen/ScreenStack.cpp
)
target_compile_features(gameLib PRIVATE cxx_std_23)
target_compile_options(gameLib PRIVATE ${BASE_COMPILE_FLAGS})
# WindowI / Game mention SFML types. Headers and defines only —
# do not link SFML (that would pull OpenGL into windowless tests).
target_include_directories(gameLib PUBLIC
    $<TARGET_PROPERTY:SFML::Graphics,INTERFACE_INCLUDE_DIRECTORIES>
)
target_compile_definitions(gameLib PUBLIC
    $<TARGET_PROPERTY:SFML::Graphics,INTERFACE_COMPILE_DEFINITIONS>
)

add_executable(game
    main.cpp
    window/WindowSFML.cpp
    time/ClockSFML.cpp
)
target_compile_features(game PRIVATE cxx_std_23)
target_compile_options(game PRIVATE ${BASE_COMPILE_FLAGS})
target_link_libraries(game PRIVATE gameLib SFML::Graphics SFML::System SFML::Window)
