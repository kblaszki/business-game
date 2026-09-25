#pragma once

#include <sgl/scene/SceneI.hpp>
#include <tetris/app/Scenes.hpp>

namespace sgl::tetris
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

} // namespace sgl::tetris
