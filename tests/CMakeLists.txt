enable_testing()

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

include(FetchGTest)
include(GoogleTest)

include_directories(${CMAKE_SOURCE_DIR}/tests)

include(AddUnitTest)

add_custom_target(build_ut)

add_subdirectory(engine/core)
add_subdirectory(engine/input)
add_subdirectory(engine/scene)
add_subdirectory(engine/loop)
add_subdirectory(engine/render)
add_subdirectory(engine/collision)
add_subdirectory(engine/resources)
add_subdirectory(engine/sfml)
add_subdirectory(arkanoid/sim)
add_subdirectory(arkanoid/app)
