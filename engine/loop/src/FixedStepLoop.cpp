#include <sgl/loop/FixedStepLoop.hpp>

namespace sgl
{

FixedStepLoop::FixedStepLoop(Seconds tick, Seconds maxFrame)
    : tick{tick}
    , maxFrame{maxFrame}
{
}

StepResult FixedStepLoop::advance(Seconds frame) noexcept
{
    if(frame > maxFrame)
    {
        frame = maxFrame;
    }

    accumulator += frame;

    std::uint32_t steps{0};
    while(accumulator >= tick)
    {
        accumulator -= tick;
        ++steps;
    }

    return StepResult{steps, accumulator / tick};
}

} // namespace sgl
