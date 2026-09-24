set(UT_NAME_SUFFIX _test)

add_custom_target(build_ut)

function(add_unit_test UNIT_TEST_NAME)
    set(multiValueArgs SOURCES DEPENDENCIES)
    cmake_parse_arguments(UNIT_TEST "" "" "${multiValueArgs}" ${ARGN})

    add_executable(${UNIT_TEST_NAME} ${UNIT_TEST_SOURCES})
    target_compile_options(${UNIT_TEST_NAME} PRIVATE ${BASE_COMPILE_FLAGS})
    target_link_libraries(${UNIT_TEST_NAME} PRIVATE ${UNIT_TEST_DEPENDENCIES} gtest gmock gtest_main)
    add_test(NAME ${UNIT_TEST_NAME} COMMAND ${UNIT_TEST_NAME})

    add_dependencies(build_ut ${UNIT_TEST_NAME})
endfunction()

add_unit_test(example_test
    SOURCES
        ExampleTest.cpp
    DEPENDENCIES
        gameLib
)

add_unit_test(game_test
    SOURCES
        GameTest.cpp
    DEPENDENCIES
        gameLib
        SFML::Graphics
)

add_unit_test(fixed_timestep_test
    SOURCES
        FixedTimestepTest.cpp
    DEPENDENCIES
        gameLib
)

add_unit_test(screen_stack_test
    SOURCES
        ScreenStackTest.cpp
    DEPENDENCIES
        gameLib
        SFML::Graphics
)

add_unit_test(menu_gameplay_test
    SOURCES
        MenuGameplayTest.cpp
    DEPENDENCIES
        gameLib
        SFML::Graphics
)

add_unit_test(input_mapper_test
    SOURCES
        InputMapperTest.cpp
    DEPENDENCIES
        gameLib
)

add_unit_test(pause_overlay_test
    SOURCES
        PauseOverlayTest.cpp
    DEPENDENCIES
        gameLib
        SFML::Graphics
)

add_unit_test(world_test
    SOURCES
        WorldTest.cpp
    DEPENDENCIES
        gameLib
        SFML::Graphics
)

add_unit_test(level_descriptor_test
    SOURCES
        LevelDescriptorTest.cpp
    DEPENDENCIES
        gameLib
        SFML::Graphics
)

add_unit_test(power_up_test
    SOURCES
        PowerUpTest.cpp
    DEPENDENCIES
        gameLib
        SFML::Graphics
)
