add_library(eng_core INTERFACE)
target_include_directories(eng_core INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_features(eng_core INTERFACE cxx_std_23)
