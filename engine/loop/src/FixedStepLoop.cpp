#include <cmath>
#include <sgl/loop/FixedStepLoop.hpp>

namespace sgl
{

FixedStepLoop::FixedStepLoop(Seconds tick, Seconds maxFrame)
    : tick{1.0 / std::round(1.0 / static_cast<double>(tick.count()))}
    , maxFrame{static_cast<double>(maxFrame.count())}
{
}

StepResult FixedStepLoop::advance(Seconds frame) noexcept
{
    Accumulator delta{static_cast<double>(frame.count())};
    if(delta.count() < 0.0)
    {
        delta = Accumulator{0.0};
    }
    if(delta > maxFrame)
    {
        delta = maxFrame;
    }

    accumulator += delta;

    std::uint32_t steps{0};
    while(accumulator >= tick)
    {
        accumulator -= tick;
        ++steps;
    }

    // Float Seconds promote with noise relative to the exact tick; drop tiny leftovers.
    if(accumulator.count() < tick.count() * 1e-6)
    {
        accumulator = Accumulator{0.0};
    }

    return StepResult{steps, static_cast<float>(accumulator / tick)};
}

} // namespace sgl
