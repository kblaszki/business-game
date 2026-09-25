add_unit_test(high_score_table_test
    SOURCES
        HighScoreTableTest.cpp
    DEPENDENCIES
        sgl_save
)

add_unit_test(save_format_test
    SOURCES
        SaveFormatTest.cpp
    DEPENDENCIES
        sgl_save
)

add_unit_test(save_file_test
    SOURCES
        SaveFileTest.cpp
    DEPENDENCIES
        sgl_save
)
