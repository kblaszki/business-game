add_library(sgl_core INTERFACE)
target_include_directories(sgl_core INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_features(sgl_core INTERFACE cxx_std_23)
