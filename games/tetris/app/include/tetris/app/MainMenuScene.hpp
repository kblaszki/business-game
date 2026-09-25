#pragma once

#include <sgl/core/Rect.hpp>
#include <sgl/core/Vec2.hpp>
#include <sgl/scene/SceneI.hpp>
#include <tetris/app/Scenes.hpp>

namespace sgl::tetris
{

class MainMenuScene : public sgl::SceneI
{
public:
    explicit MainMenuScene(const AppServices& services);

    void update(sgl::SceneContext& ctx, sgl::Seconds dt) override;
    void render(sgl::RenderQueue& queue) const override;
    sgl::SceneTraits traits() const override;

private:
    [[nodiscard]] bool hitStart(sgl::Vec2f point) const;
    [[nodiscard]] bool hitQuit(sgl::Vec2f point) const;

    AppServices services_;
    sgl::Rect<float> startButton_{};
    sgl::Rect<float> quitButton_{};
    bool startHover_{false};
    bool quitHover_{false};
};

} // namespace sgl::tetris
