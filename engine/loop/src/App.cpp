#include <sgl/core/Features.hpp>
#include <sgl/core/Time.hpp>
#include <sgl/loop/App.hpp>
#include <sgl/render/RenderQueue.hpp>
#include <sgl/render/RendererI.hpp>
#include <sgl/scene/SceneStack.hpp>
#include <utility>

namespace sgl
{

App::App(PlatformI& platform, ClockI& clock, ActionMap map, SceneStack& stack, RenderQueue& queue)
    : platform{platform}
    , clock{clock}
    , map{std::move(map)}
    , stack{stack}
    , queue{queue}
{
}

void App::run()
{
    while(platform.isOpen())
    {
        runFrame();
    }
}

void App::runFrames(std::uint32_t n)
{
    for(std::uint32_t i{0}; i < n; ++i)
    {
        if(!platform.isOpen())
        {
            return;
        }
        runFrame();
    }
}

float App::lastAlpha() const
{
    return alpha;
}

void App::runFrame()
{
    if(edgesConsumed_)
    {
        input.beginFrame();
    }
    edgesConsumed_ = false;

    while(const std::optional<InputEvent> event = platform.poll())
    {
        input.apply(*event, map);
    }

    if(input.closeRequested())
    {
        platform.close();
    }

    const StepResult step = loop.advance(clock.restart());
    for(std::uint32_t i{0}; i < step.steps; ++i)
    {
        if(i > 0)
        {
            input.beginFrame();
        }
        stack.update(input, kTick);
        edgesConsumed_ = true;
    }

    if(stack.quitRequested() || stack.empty())
    {
        platform.close();
    }

    queue.clear();
    stack.render(queue);

    RendererI& renderer = platform.renderer();
    renderer.begin();
    renderer.submit(queue);
    renderer.end();

    alpha = step.alpha;
}

} // namespace sgl
