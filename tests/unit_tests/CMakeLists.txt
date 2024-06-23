add_executable(controllers_test
    EventControllerTest.cpp
    GameControllerTest.cpp
)
target_compile_options(controllers_test PRIVATE ${BASE_COMPILE_FLAGS})
target_link_libraries(controllers_test PRIVATE gameLib gtest gmock gtest_main)
add_test(NAME controllers_test COMMAND controllers_test)

add_executable(managers_test
    managers/ClosedEventManagerTest.cpp
    managers/KeyPressedManagerTest.cpp
    managers/KeyReleasedManagerTest.cpp
)
target_compile_options(managers_test PRIVATE ${BASE_COMPILE_FLAGS})
target_link_libraries(managers_test PRIVATE gameLib gtest gmock gtest_main)
add_test(NAME managers_test COMMAND managers_test)