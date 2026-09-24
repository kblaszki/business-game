#include <arkanoid/app/Bindings.hpp>

#include <eng/input/Key.hpp>

namespace arkanoid
{

Actions makeActions()
{
    return Actions{
        .confirm = eng::ActionId{.id = 1},
        .cancel = eng::ActionId{.id = 2},
        .pause = eng::ActionId{.id = 3},
        .paddle = eng::AxisId{.id = 4},
    };
}

eng::ActionMap defaultBindings(const Actions& actions)
{
    eng::ActionMap map;
    map.bind(eng::Key::Enter, actions.confirm);
    map.bind(eng::Key::Escape, actions.pause);
    map.bind(eng::Key::Backspace, actions.cancel);
    map.bindAxis(eng::Key::Left, eng::Key::Right, actions.paddle);
    map.bindAxis(eng::Key::A, eng::Key::D, actions.paddle);
    return map;
}

} // namespace arkanoid
