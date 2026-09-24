#pragma once

#include <arkanoid/app/Scenes.hpp>
#include <eng/scene/SceneI.hpp>

namespace arkanoid
{

class PauseScene : public eng::SceneI
{
public:
    explicit PauseScene(const AppServices& services);

    void update(eng::SceneContext& ctx, eng::Seconds dt) override;
    void render(eng::RenderQueue& queue) const override;
    eng::SceneTraits traits() const override;

private:
    AppServices services_;
};

} // namespace arkanoid
