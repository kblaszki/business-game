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
