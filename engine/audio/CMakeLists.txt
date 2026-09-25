add_library(sgl_audio INTERFACE)
target_include_directories(sgl_audio INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_features(sgl_audio INTERFACE cxx_std_23)
target_link_libraries(sgl_audio INTERFACE sgl_core)
