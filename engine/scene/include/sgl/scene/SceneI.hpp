#pragma once

#include <sgl/core/Time.hpp>
#include <sgl/scene/SceneTraits.hpp>

namespace sgl
{

class SceneContext;
class RenderQueue;

class SceneI
{
public:
    virtual ~SceneI() = default;

    virtual void update(SceneContext& ctx, Seconds dt) = 0;
    virtual void render(RenderQueue& queue) const = 0;
    virtual SceneTraits traits() const = 0;
};

} // namespace sgl
