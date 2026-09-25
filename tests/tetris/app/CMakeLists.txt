add_unit_test(tetris_input_mapping_test
    SOURCES
        InputMappingTest.cpp
    DEPENDENCIES
        tetris_app
)

add_unit_test(tetris_board_render_test
    SOURCES
        BoardRenderTest.cpp
    DEPENDENCIES
        tetris_app
        sgl_render
)

add_unit_test(tetris_hud_test
    SOURCES
        HudTest.cpp
    DEPENDENCIES
        tetris_app
)

add_unit_test(tetris_scenes_test
    SOURCES
        ScenesTest.cpp
    DEPENDENCIES
        tetris_app
)

add_unit_test(tetris_sounds_test
    SOURCES
        SoundsTest.cpp
    DEPENDENCIES
        tetris_app
)

add_unit_test(tetris_feedback_test
    SOURCES
        FeedbackTest.cpp
    DEPENDENCIES
        tetris_app
)
