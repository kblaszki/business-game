#include <arkanoid/app/HudModel.hpp>

#include <eng/core/Time.hpp>

#include <string>

namespace arkanoid
{

HudModel makeHud(const State& state)
{
    HudModel hud;
    hud.score = "Score " + std::to_string(state.score);
    hud.lives = "Lives " + std::to_string(state.lives);

    if(state.effects.timed.has_value() && state.effects.remaining > eng::Seconds{})
    {
        switch(*state.effects.timed)
        {
            case PowerUpKind::Wide:
                hud.effect = "Wide";
                break;
            case PowerUpKind::Slow:
                hud.effect = "Slow";
                break;
            case PowerUpKind::MultiBall:
            case PowerUpKind::ExtraLife:
                break;
        }
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

} // namespace arkanoid
