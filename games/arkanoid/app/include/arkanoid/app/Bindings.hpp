#pragma once

#include <eng/input/ActionId.hpp>
#include <eng/input/ActionMap.hpp>

namespace arkanoid
{

struct Actions
{
    eng::ActionId confirm{};
    eng::ActionId cancel{};
    eng::ActionId pause{};
    eng::AxisId paddle{};
};

[[nodiscard]] Actions makeActions();
[[nodiscard]] eng::ActionMap defaultBindings(const Actions& actions);

} // namespace arkanoid
