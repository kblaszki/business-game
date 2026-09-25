add_library(sgl_input STATIC
    src/ActionMap.cpp
    src/InputState.cpp
)
target_include_directories(sgl_input PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_features(sgl_input PUBLIC cxx_std_23)
target_compile_options(sgl_input PRIVATE ${BASE_COMPILE_FLAGS})
target_link_libraries(sgl_input PUBLIC sgl_core)
