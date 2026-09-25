#pragma once

#include <sgl/core/Vec2.hpp>

namespace sgl
{

struct Contact
{
    Vec2f normal{};
    float depth{};
};

} // namespace sgl
