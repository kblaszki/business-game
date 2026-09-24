add_library(eng_input STATIC
    src/ActionMap.cpp
    src/InputState.cpp
)
target_include_directories(eng_input PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_features(eng_input PUBLIC cxx_std_23)
target_compile_options(eng_input PRIVATE ${BASE_COMPILE_FLAGS})
target_link_libraries(eng_input PUBLIC eng_core)
