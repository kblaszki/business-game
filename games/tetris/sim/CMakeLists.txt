add_library(tetris_sim STATIC
    src/Grid.cpp
    src/Piece.cpp
    src/Bag.cpp
    src/Scoring.cpp
    src/TetrisGame.cpp
)
target_include_directories(tetris_sim PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_features(tetris_sim PUBLIC cxx_std_23)
target_compile_options(tetris_sim PRIVATE ${BASE_COMPILE_FLAGS})
target_link_libraries(tetris_sim PUBLIC sgl_core)
