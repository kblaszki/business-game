#pragma once

#include <arkanoid/sim/Events.hpp>
#include <arkanoid/sim/State.hpp>
#include <sgl/core/Time.hpp>
#include <vector>

namespace sgl::arkanoid
{

struct SimInput
{
    float paddleAxis{};
    bool launch{};
};

[[nodiscard]] std::vector<SimEvent> step(State& state, const SimInput& input, sgl::Seconds dt);

void applyPowerUp(State& state, PowerUpKind kind);

} // namespace sgl::arkanoid
