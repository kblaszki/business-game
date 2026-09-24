add_unit_test(arkanoid_assets_test
    SOURCES
        AssetsTest.cpp
    DEPENDENCIES
        arkanoid_app
)

add_unit_test(arkanoid_hud_test
    SOURCES
        HudModelTest.cpp
    DEPENDENCIES
        arkanoid_app
)

add_unit_test(arkanoid_scene_render_test
    SOURCES
        SceneRenderTest.cpp
    DEPENDENCIES
        arkanoid_app
        eng_render
)
