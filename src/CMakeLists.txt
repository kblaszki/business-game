add_subdirectory(sum-library)

add_executable(game main.cpp)
target_compile_features(game PRIVATE cxx_std_20)
target_compile_options(game PRIVATE ${BASE_COMPILE_FLAGS})
target_link_libraries(game PRIVATE sfml-graphics sfml-system sfml-window)

add_custom_command(TARGET game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    $<TARGET_FILE_DIR:sfml-system> $<TARGET_FILE_DIR:game>
    COMMENT "Copying SFML DLLs to output directory"
)