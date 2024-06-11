enable_testing()

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

include(FetchGTest)
include(GoogleTest)

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/tests)

add_executable(sum_library_test test.cpp)
target_compile_options(sum_library_test PRIVATE ${BASE_COMPILE_FLAGS})
target_link_libraries(sum_library_test PRIVATE sumLibrary gtest gtest_main)
add_test(NAME sum_library_test COMMAND sum_library_test)

add_subdirectory(unit_tests)