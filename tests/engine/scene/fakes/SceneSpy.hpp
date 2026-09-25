#pragma once

#include <cstdint>
#include <functional>
#include <sgl/core/Time.hpp>
#include <sgl/scene/SceneContext.hpp>
#include <sgl/scene/SceneI.hpp>
#include <sgl/scene/SceneRequest.hpp>
#include <sgl/scene/SceneTraits.hpp>

namespace sgl
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

} // namespace sgl
