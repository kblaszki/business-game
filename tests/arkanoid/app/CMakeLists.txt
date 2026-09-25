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
        sgl_render
)

add_unit_test(arkanoid_scenes_test
    SOURCES
        ScenesTest.cpp
    DEPENDENCIES
        arkanoid_app
)

add_unit_test(arkanoid_result_scene_test
    SOURCES
        ResultSceneTest.cpp
    DEPENDENCIES
        arkanoid_app
        sgl_render
)

add_unit_test(arkanoid_sounds_test
    SOURCES
        SoundsTest.cpp
    DEPENDENCIES
        arkanoid_app
)

add_unit_test(arkanoid_feedback_test
    SOURCES
        FeedbackTest.cpp
    DEPENDENCIES
        arkanoid_app
)
