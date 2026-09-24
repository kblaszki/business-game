#pragma once

#include <eng/core/Vec2.hpp>

namespace eng
{

struct Contact
{
    Vec2f normal{};
    float depth{};
};

} // namespace eng
