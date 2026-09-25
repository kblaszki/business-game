#pragma once

#include <arkanoid/app/Scenes.hpp>
#include <cstdint>
#include <optional>
#include <sgl/core/Rect.hpp>
#include <sgl/core/Vec2.hpp>
#include <sgl/scene/SceneI.hpp>

namespace sgl::arkanoid
{

class ResultScene : public sgl::SceneI
{
public:
    ResultScene(const AppServices& services,
                Outcome outcome,
                std::uint32_t score,
                std::optional<std::size_t> rank,
                bool saveFailed);

    void update(sgl::SceneContext& ctx, sgl::Seconds dt) override;
    void render(sgl::RenderQueue& queue) const override;
    sgl::SceneTraits traits() const override;

    [[nodiscard]] Outcome outcome() const noexcept;
    [[nodiscard]] std::uint32_t score() const noexcept;
    [[nodiscard]] std::optional<std::size_t> rank() const noexcept;
    [[nodiscard]] bool saveFailed() const noexcept;

private:
    [[nodiscard]] bool hitRetry(sgl::Vec2f point) const;
    [[nodiscard]] bool hitMenu(sgl::Vec2f point) const;

    AppServices services_;
    Outcome outcome_{Outcome::Lost};
    std::uint32_t score_{};
    std::optional<std::size_t> rank_{};
    bool saveFailed_{false};
    sgl::Rect<float> retryButton_{};
    sgl::Rect<float> menuButton_{};
    bool retryHover_{false};
    bool menuHover_{false};
};

} // namespace sgl::arkanoid
