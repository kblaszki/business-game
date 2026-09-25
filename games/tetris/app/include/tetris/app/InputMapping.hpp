#pragma once

#include <sgl/input/InputState.hpp>
#include <tetris/app/Bindings.hpp>
#include <tetris/sim/TetrisGame.hpp>

namespace sgl::tetris
{

[[nodiscard]] TetrisInput toTetrisInput(const sgl::InputState& input, const Actions& actions);

} // namespace sgl::tetris
