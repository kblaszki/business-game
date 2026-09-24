#pragma once

#include <arkanoid/app/HudModel.hpp>
#include <arkanoid/app/Scenes.hpp>
#include <arkanoid/sim/State.hpp>
#include <eng/scene/SceneI.hpp>

namespace arkanoid
{

class GameplayScene : public eng::SceneI
{
public:
    GameplayScene(const AppServices& services, StageId stage);

    void update(eng::SceneContext& ctx, eng::Seconds dt) override;
    void render(eng::RenderQueue& queue) const override;
    eng::SceneTraits traits() const override;

    [[nodiscard]] State& state();
    [[nodiscard]] const State& state() const;
    [[nodiscard]] HudModel hud() const;

private:
    void restart();
    void advanceFrom(StageId cleared);

    AppServices services_;
    State state_;
};

} // namespace arkanoid
