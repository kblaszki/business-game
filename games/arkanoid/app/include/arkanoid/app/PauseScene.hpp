#pragma once

#include <arkanoid/app/Scenes.hpp>
#include <sgl/scene/SceneI.hpp>

namespace sgl::arkanoid
{

class PauseScene : public sgl::SceneI
{
public:
    explicit PauseScene(const AppServices& services);

    void update(sgl::SceneContext& ctx, sgl::Seconds dt) override;
    void render(sgl::RenderQueue& queue) const override;
    sgl::SceneTraits traits() const override;

private:
    AppServices services_;
};

} // namespace sgl::arkanoid
