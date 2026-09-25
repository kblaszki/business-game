#pragma once

#include <sgl/scene/SceneI.hpp>
#include <tetris/app/BoardRender.hpp>
#include <tetris/app/HudModel.hpp>
#include <tetris/app/Scenes.hpp>
#include <tetris/sim/TetrisGame.hpp>

namespace sgl::tetris
{

class PlayScene : public sgl::SceneI
{
public:
    explicit PlayScene(const AppServices& services);

    void update(sgl::SceneContext& ctx, sgl::Seconds dt) override;
    void render(sgl::RenderQueue& queue) const override;
    sgl::SceneTraits traits() const override;

    [[nodiscard]] TetrisGame& game();
    [[nodiscard]] const TetrisGame& game() const;
    [[nodiscard]] HudModel hud() const;

private:
    AppServices services_;
    Layout layout_;
    TetrisGame game_;
};

} // namespace sgl::tetris
