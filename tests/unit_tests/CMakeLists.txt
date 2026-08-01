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

add_unit_test(controllers_test
    SOURCES
        controllers/EventControllerTest.cpp
        controllers/GameControllerTest.cpp
    DEPENDENCIES 
        gameLib
)

add_unit_test(handlers_test
    SOURCES
        handlers/OnClickHandlerTest.cpp
        handlers/OnHoverHandlerTest.cpp
    DEPENDENCIES 
        gameLib
)

add_unit_test(managers_test
    SOURCES
        managers/GameExitManagerTest.cpp
        managers/KeyboardManagerTest.cpp
        managers/MouseManagerTest.cpp
    DEPENDENCIES 
        gameLib
)

add_unit_test(resources_test
    SOURCES
        resources/ResourceManagerTest.cpp
    DEPENDENCIES
        gameLib
)
target_compile_definitions(resources_test PRIVATE
    RESOURCES_ROOT_FOR_TESTS="${CMAKE_SOURCE_DIR}/resources"
)

add_unit_test(entities_test
    SOURCES
        entities/BallTest.cpp
        entities/BrickTest.cpp
        entities/PaddleTest.cpp
        entities/ButtonTest.cpp
    DEPENDENCIES
        gameLib
)
target_compile_definitions(entities_test PRIVATE
    RESOURCES_ROOT_FOR_TESTS="${CMAKE_SOURCE_DIR}/resources"
)

add_unit_test(utils_test
    SOURCES
        utils/RectCollisionTest.cpp
    DEPENDENCIES
        gameLib
)

add_unit_test(screens_test
    SOURCES
        screens/MenuScreenTest.cpp
        screens/GameScreenTest.cpp
    DEPENDENCIES
        gameLib
)
target_compile_definitions(screens_test PRIVATE
    RESOURCES_ROOT_FOR_TESTS="${CMAKE_SOURCE_DIR}/resources"
)