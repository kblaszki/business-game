#pragma once

#include <arkanoid/app/Scenes.hpp>
#include <sgl/core/Rect.hpp>
#include <sgl/core/Vec2.hpp>
#include <sgl/scene/SceneI.hpp>

namespace sgl::arkanoid
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

} // namespace sgl::arkanoid
