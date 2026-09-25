add_unit_test(tetris_grid_test
    SOURCES
        GridTest.cpp
    DEPENDENCIES
        tetris_sim
)

add_unit_test(tetris_srs_test
    SOURCES
        SrsTest.cpp
    DEPENDENCIES
        tetris_sim
)

add_unit_test(tetris_bag_test
    SOURCES
        BagTest.cpp
    DEPENDENCIES
        tetris_sim
)

add_unit_test(tetris_scoring_test
    SOURCES
        ScoringTest.cpp
    DEPENDENCIES
        tetris_sim
)

add_unit_test(tetris_game_test
    SOURCES
        TetrisGameTest.cpp
    DEPENDENCIES
        tetris_sim
)

add_unit_test(tetris_property_test
    SOURCES
        TetrisPropertyTest.cpp
    DEPENDENCIES
        tetris_sim
)
