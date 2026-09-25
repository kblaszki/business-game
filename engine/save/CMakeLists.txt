add_library(sgl_save INTERFACE)
target_include_directories(sgl_save INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_features(sgl_save INTERFACE cxx_std_23)
target_link_libraries(sgl_save INTERFACE sgl_core)
