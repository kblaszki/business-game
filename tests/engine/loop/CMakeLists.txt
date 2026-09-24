add_unit_test(fixed_step_loop_test
    SOURCES
        FixedStepLoopTest.cpp
    DEPENDENCIES
        eng_loop
)

add_unit_test(app_test
    SOURCES
        AppTest.cpp
        ${CMAKE_SOURCE_DIR}/tests/engine/scene/fakes/SceneSpy.cpp
    DEPENDENCIES
        eng_loop
)
