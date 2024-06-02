enable_testing()

include(FetchGTest)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

include(GoogleTest)

add_executable(sum_library_test test.cpp)
target_compile_options(sum_library_test PRIVATE ${BASE_COMPILE_FLAGS})
target_link_libraries(sum_library_test PRIVATE sumLibrary gtest_main)
add_test(NAME sum_library_test COMMAND sum_library_test)