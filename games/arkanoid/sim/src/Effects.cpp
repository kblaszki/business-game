#include <arkanoid/sim/Effects.hpp>
#include <arkanoid/sim/Tuning.hpp>
#include <variant>

namespace sgl::arkanoid
{

float paddleWidth(const Effects& effects)
{
    for(const TimedEffect& effect: effects.active)
    {
        if(std::holds_alternative<Wide>(effect))
        {
            return paddleWideWidth;
        }
    }
    return paddleNormalWidth;
}

float ballSpeedMultiplier(const Effects& effects)
{
    for(const TimedEffect& effect: effects.active)
    {
        if(std::holds_alternative<Slow>(effect))
        {
            return slowFactor;
        }
    }
    return 1.f;
}

} // namespace sgl::arkanoid
