#pragma once

#include <arkanoid/app/HudModel.hpp>
#include <arkanoid/app/Scenes.hpp>
#include <arkanoid/sim/State.hpp>
#include <sgl/scene/SceneI.hpp>

namespace sgl::arkanoid
{

class GameplayScene : public sgl::SceneI
{
public:
    GameplayScene(const AppServices& services, StageId stage);

    void update(sgl::SceneContext& ctx, sgl::Seconds dt) override;
    void render(sgl::RenderQueue& queue) const override;
    sgl::SceneTraits traits() const override;

    [[nodiscard]] State& state();
    [[nodiscard]] const State& state() const;
    [[nodiscard]] HudModel hud() const;

private:
    void restart();
    void advanceFrom(StageId cleared);

    AppServices services_;
    State state_;
};

} // namespace sgl::arkanoid
