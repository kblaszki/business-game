#include <eng/core/Time.hpp>
#include <eng/loop/App.hpp>
#include <eng/render/RenderQueue.hpp>
#include <eng/render/RendererI.hpp>
#include <eng/scene/SceneStack.hpp>
#include <utility>

namespace eng
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
    input.beginFrame();
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
    }

    if(stack.quitRequested())
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

} // namespace eng
