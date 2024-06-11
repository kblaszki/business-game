

add_executable(controllers_test GameControllerTest.cpp)
target_compile_options(controllers_test PRIVATE ${BASE_COMPILE_FLAGS})
target_link_libraries(controllers_test PRIVATE gameLib gtest gmock gtest_main)
add_test(NAME controllers_test COMMAND controllers_test)