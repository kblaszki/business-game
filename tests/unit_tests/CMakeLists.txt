set(UT_NAME_SUFFIX _test)

add_custom_target(build_ut)

function(add_unit_test UNIT_TEST_NAME)
    set(multiValueArgs SOURCES DEPENDENCIES)
    cmake_parse_arguments(UNIT_TEST "" "" "${multiValueArgs}" ${ARGN})

    add_executable(${UNIT_TEST_NAME} ${UNIT_TEST_SOURCES})
    target_compile_options(${UNIT_TEST_NAME} PRIVATE ${BASE_COMPILE_FLAGS})
    target_link_libraries(${UNIT_TEST_NAME} PRIVATE ${UNIT_TEST_DEPENDENCIES} gtest gmock gtest_main)
    # GNU ld may pull only part of static HarfBuzz before FreeType adds more undefs.
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND TARGET harfbuzz)
        target_link_libraries(${UNIT_TEST_NAME} PRIVATE
            -Wl,--whole-archive $<TARGET_FILE:harfbuzz> -Wl,--no-whole-archive
        )
    endif()
    add_test(NAME ${UNIT_TEST_NAME} COMMAND ${UNIT_TEST_NAME})

    add_dependencies(build_ut ${UNIT_TEST_NAME})
endfunction()

add_unit_test(smoke_test
    SOURCES
        GameSmokeTest.cpp
    DEPENDENCIES
        gameLib
)

add_unit_test(fixed_timestep_test
    SOURCES
        FixedTimestepTest.cpp
    DEPENDENCIES
        gameLib
)

add_unit_test(iscreen_dummy_test
    SOURCES
        IScreenDummyTest.cpp
    DEPENDENCIES
        gameLib
)

add_unit_test(screen_stack_test
    SOURCES
        ScreenStackTest.cpp
    DEPENDENCIES
        gameLib
)

add_unit_test(screen_transition_test
    SOURCES
        ScreenTransitionTest.cpp
    DEPENDENCIES
        gameLib
)

add_unit_test(input_mapper_test
    SOURCES
        InputMapperTest.cpp
    DEPENDENCIES
        gameLib
)

add_unit_test(pause_blocks_ticks_test
    SOURCES
        PauseBlocksTicksTest.cpp
    DEPENDENCIES
        gameLib
)

add_unit_test(world_test
    SOURCES
        WorldTest.cpp
    DEPENDENCIES
        gameLib
)

add_unit_test(rect_collision_test
    SOURCES
        RectCollisionTest.cpp
    DEPENDENCIES
        gameLib
)

add_unit_test(arkanoid_session_test
    SOURCES
        ArkanoidSessionTest.cpp
    DEPENDENCIES
        gameLib
)
