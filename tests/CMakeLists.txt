enable_testing()

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

include(FetchGTest)
include(GoogleTest)

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/tests)

add_subdirectory(unit_tests)