#include <arkanoid/app/Bindings.hpp>
#include <sgl/input/Key.hpp>

namespace sgl::arkanoid
{

Actions makeActions()
{
    return Actions{
        .confirm = sgl::ActionId{.id = 1},
        .cancel = sgl::ActionId{.id = 2},
        .pause = sgl::ActionId{.id = 3},
        .paddle = sgl::AxisId{.id = 4},
    };
}

sgl::ActionMap defaultBindings(const Actions& actions)
{
    sgl::ActionMap map;
    map.bind(sgl::Key::Enter, actions.confirm);
    map.bind(sgl::Key::Escape, actions.pause);
    map.bind(sgl::Key::Backspace, actions.cancel);
    map.bindAxis(sgl::Key::Left, sgl::Key::Right, actions.paddle);
    map.bindAxis(sgl::Key::A, sgl::Key::D, actions.paddle);
    return map;
}

} // namespace sgl::arkanoid
