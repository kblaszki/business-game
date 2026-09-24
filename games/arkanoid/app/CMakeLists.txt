add_library(arkanoid_app STATIC
    src/Assets.cpp
    src/Bindings.cpp
    src/HudModel.cpp
    src/SceneRender.cpp
    src/Scenes.cpp
    src/MainMenuScene.cpp
    src/GameplayScene.cpp
    src/PauseScene.cpp
)
target_include_directories(arkanoid_app PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_features(arkanoid_app PUBLIC cxx_std_23)
target_compile_options(arkanoid_app PRIVATE ${BASE_COMPILE_FLAGS})
target_link_libraries(arkanoid_app PUBLIC arkanoid_sim eng_loop eng_resources)
