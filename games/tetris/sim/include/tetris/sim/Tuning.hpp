#pragma once

#include <sgl/core/Time.hpp>

namespace sgl::tetris
{

struct Tuning
{
    sgl::Seconds das{0.167f};
    sgl::Seconds arr{0.033f};
    sgl::Seconds lockDelay{0.5f};
    int maxLockResets{15};
    float softDropFactor{20.f};
};

} // namespace sgl::tetris
