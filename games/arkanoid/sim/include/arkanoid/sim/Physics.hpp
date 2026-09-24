#pragma once

#include <arkanoid/sim/Events.hpp>
#include <arkanoid/sim/State.hpp>
#include <eng/core/Time.hpp>

#include <vector>

namespace arkanoid
{

struct SimInput
{
    float paddleAxis{};
    bool launch{};
};

[[nodiscard]] std::vector<SimEvent> step(State& state, const SimInput& input, eng::Seconds dt);

void applyPowerUp(State& state, PowerUpKind kind);

} // namespace arkanoid
