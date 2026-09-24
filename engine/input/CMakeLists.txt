add_library(eng_input INTERFACE)
target_include_directories(eng_input INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_features(eng_input INTERFACE cxx_std_23)
target_link_libraries(eng_input INTERFACE eng_core)
