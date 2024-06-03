enable_testing()

include(FetchGTest)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

include(GoogleTest)

add_executable(sum_library_test test.cpp)
target_compile_options(sum_library_test PRIVATE ${BASE_COMPILE_FLAGS})
target_link_libraries(sum_library_test PRIVATE sumLibrary gtest gtest_main)
add_test(NAME sum_library_test COMMAND sum_library_test)

add_custom_command(TARGET sum_library_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    $<TARGET_FILE_DIR:gtest> $<TARGET_FILE_DIR:sum_library_test>
    COMMENT "Copying GTEST DLLs to test output directory"
)

add_custom_command(TARGET sum_library_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    $<TARGET_FILE_DIR:sumLibrary> $<TARGET_FILE_DIR:sum_library_test>
    COMMENT "Copying sumLibrary DLLs to test output directory"
)