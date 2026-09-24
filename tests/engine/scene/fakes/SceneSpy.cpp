#include <engine/scene/fakes/SceneSpy.hpp>

namespace eng
{

void SceneSpy::update(SceneContext& ctx, Seconds dt)
{
    ++updateCount;
    lastDt = dt;
    if(onUpdate)
    {
        onUpdate(ctx);
    }
}

void SceneSpy::render(RenderQueue&) const {}

SceneTraits SceneSpy::traits() const
{
    return traits_;
}

} // namespace eng
