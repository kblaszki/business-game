#pragma once

#include <sgl/input/ActionId.hpp>
#include <sgl/input/ActionMap.hpp>

namespace sgl::arkanoid
{

struct Actions
{
    sgl::ActionId confirm{};
    sgl::ActionId cancel{};
    sgl::ActionId pause{};
    sgl::AxisId paddle{};
};

[[nodiscard]] Actions makeActions();
[[nodiscard]] sgl::ActionMap defaultBindings(const Actions& actions);

} // namespace sgl::arkanoid
