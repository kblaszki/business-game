add_unit_test(event_translate_test
    SOURCES
        EventTranslateTest.cpp
    DEPENDENCIES
        sgl_input
        SFML::Window
)
target_include_directories(event_translate_test PRIVATE
    ${CMAKE_SOURCE_DIR}/engine/sfml/include
)

add_unit_test(sfml_draw_test
    SOURCES
        SfmlDrawTest.cpp
    DEPENDENCIES
        sgl_render
        SFML::Graphics
)
target_include_directories(sfml_draw_test PRIVATE
    ${CMAKE_SOURCE_DIR}/engine/sfml/include
)
