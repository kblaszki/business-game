#pragma once

#include <cstdint>
#include <eng/core/Time.hpp>
#include <eng/scene/SceneContext.hpp>
#include <eng/scene/SceneI.hpp>
#include <eng/scene/SceneRequest.hpp>
#include <eng/scene/SceneTraits.hpp>
#include <functional>

namespace eng
{

class SceneSpy : public SceneI
{
public:
    void update(SceneContext& ctx, Seconds dt) override;
    void render(RenderQueue& queue) const override;
    [[nodiscard]] SceneTraits traits() const override;

    SceneTraits traits_{};
    std::uint32_t updateCount{};
    Seconds lastDt{};
    std::move_only_function<void(SceneContext&)> onUpdate{};
};

} // namespace eng
