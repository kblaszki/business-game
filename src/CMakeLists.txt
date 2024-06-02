add_library(sumLibrary STATIC LibraryCode.cpp)
target_compile_options(sumLibrary PRIVATE ${BASE_COMPILE_FLAGS})

add_executable(game main.cpp)
target_compile_features(game PRIVATE cxx_std_20)
target_compile_options(game PRIVATE ${BASE_COMPILE_FLAGS})
target_link_libraries(game PRIVATE sfml-graphics sfml-system sfml-window)