add_library(eng_resources INTERFACE)
target_include_directories(eng_resources INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_features(eng_resources INTERFACE cxx_std_23)
target_link_libraries(eng_resources INTERFACE eng_core)
