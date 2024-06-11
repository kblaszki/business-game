

add_executable(core_test GameControllerTest.cpp)
target_compile_options(core_test PRIVATE ${BASE_COMPILE_FLAGS})
target_link_libraries(core_test PRIVATE gameLib gtest gmock gtest_main)
add_test(NAME core_test COMMAND core_test)