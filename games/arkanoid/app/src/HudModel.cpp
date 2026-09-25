#include <arkanoid/app/HudModel.hpp>
#include <arkanoid/sim/Effects.hpp>
#include <cmath>
#include <sgl/core/Overloaded.hpp>
#include <sgl/core/Time.hpp>
#include <string>
#include <variant>

namespace sgl::arkanoid
{

HudModel makeHud(const State& state)
{
    HudModel hud;
    hud.score = "Score " + std::to_string(state.score);
    hud.lives = "Lives " + std::to_string(state.lives);

    for(const TimedEffect& effect: state.effects.active)
    {
        std::visit(sgl::Overloaded{
                       [&](const Wide& wide) {
                           if(wide.remaining > sgl::Seconds{})
                           {
                               if(!hud.effect.empty())
                               {
                                   hud.effect.push_back(' ');
                               }
                               hud.effect +=
                                   "Wide " + std::to_string(static_cast<int>(std::floor(wide.remaining.count())));
                           }
                       },
                       [&](const Slow& slow) {
                           if(slow.remaining > sgl::Seconds{})
                           {
                               if(!hud.effect.empty())
                               {
                                   hud.effect.push_back(' ');
                               }
                               hud.effect +=
                                   "Slow " + std::to_string(static_cast<int>(std::floor(slow.remaining.count())));
                           }
                       },
                   },
                   effect);
    }

    if(state.cleared && state.stage == StageId::Stage3)
    {
        hud.banner = "You win";
    }
    else if(state.over)
    {
        hud.banner = "You lose";
    }

    if(hud.banner.has_value())
    {
        hud.hint = "Enter - retry   Backspace - menu";
    }

    return hud;
}

} // namespace sgl::arkanoid
