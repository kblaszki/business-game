#pragma once

#include <cstdint>
#include <sgl/core/Time.hpp>

namespace sgl
{

struct StepResult
{
    std::uint32_t steps{};
    float alpha{};
};

class FixedStepLoop
{
public:
    explicit FixedStepLoop(Seconds tick = kTick, Seconds maxFrame = Seconds{0.25f});

    StepResult advance(Seconds frame) noexcept;

private:
    Seconds tick;
    Seconds maxFrame;
    Seconds accumulator{};
};

} // namespace sgl
