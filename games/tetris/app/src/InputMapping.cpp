#include <tetris/app/InputMapping.hpp>

namespace sgl::tetris
{

TetrisInput toTetrisInput(const sgl::InputState& input, const Actions& actions)
{
    return TetrisInput{
        .leftPressed = input.action(actions.left).pressed,
        .leftHeld = input.action(actions.left).held,
        .rightPressed = input.action(actions.right).pressed,
        .rightHeld = input.action(actions.right).held,
        .softDropHeld = input.action(actions.softDrop).held,
        .hardDrop = input.action(actions.hardDrop).pressed,
        .rotateCw = input.action(actions.rotateCw).pressed,
        .rotateCcw = input.action(actions.rotateCcw).pressed,
        .hold = input.action(actions.hold).pressed,
    };
}

} // namespace sgl::tetris
