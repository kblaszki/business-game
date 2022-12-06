include(FetchGTest)
include(GoogleTest)

add_executable(testCommon test.cpp)
target_compile_options(testCommon PRIVATE ${BASE_COMPILE_FLAGS})
target_link_libraries(testCommon PRIVATE sumLibrary gtest_main)
gtest_discover_tests(testCommon)