#pragma once

#include <arkanoid/app/Scenes.hpp>
#include <eng/core/Rect.hpp>
#include <eng/core/Vec2.hpp>
#include <eng/scene/SceneI.hpp>

namespace arkanoid
{

class MainMenuScene : public eng::SceneI
{
public:
    explicit MainMenuScene(const AppServices& services);

    void update(eng::SceneContext& ctx, eng::Seconds dt) override;
    void render(eng::RenderQueue& queue) const override;
    eng::SceneTraits traits() const override;

private:
    [[nodiscard]] bool hitStart(eng::Vec2f point) const;
    [[nodiscard]] bool hitQuit(eng::Vec2f point) const;

    AppServices services_;
    eng::Rect<float> startButton_{};
    eng::Rect<float> quitButton_{};
    bool startHover_{false};
    bool quitHover_{false};
};

} // namespace arkanoid
