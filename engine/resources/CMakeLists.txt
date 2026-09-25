add_library(sgl_resources INTERFACE)
target_include_directories(sgl_resources INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_features(sgl_resources INTERFACE cxx_std_23)
target_link_libraries(sgl_resources INTERFACE sgl_core)
