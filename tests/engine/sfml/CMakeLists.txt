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
