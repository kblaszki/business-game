#pragma once

#include <cstdint>
#include <sgl/core/Rect.hpp>
#include <sgl/core/Vec2.hpp>
#include <sgl/scene/SceneI.hpp>
#include <tetris/app/Scenes.hpp>

namespace sgl::tetris
{

class GameOverScene : public sgl::SceneI
{
public:
    GameOverScene(const AppServices& services, std::uint32_t score);

    void update(sgl::SceneContext& ctx, sgl::Seconds dt) override;
    void render(sgl::RenderQueue& queue) const override;
    sgl::SceneTraits traits() const override;

    [[nodiscard]] std::uint32_t score() const noexcept;

private:
    [[nodiscard]] bool hitRetry(sgl::Vec2f point) const;
    [[nodiscard]] bool hitMenu(sgl::Vec2f point) const;

    AppServices services_;
    std::uint32_t score_{};
    sgl::Rect<float> retryButton_{};
    sgl::Rect<float> menuButton_{};
    bool retryHover_{false};
    bool menuHover_{false};
};

} // namespace sgl::tetris
