#include <arkanoid/app/PauseScene.hpp>
#include <arkanoid/app/Theme.hpp>
#include <arkanoid/sim/Tuning.hpp>
#include <sgl/render/DrawCommand.hpp>
#include <sgl/render/RenderQueue.hpp>
#include <sgl/scene/SceneContext.hpp>

namespace sgl::arkanoid
{

PauseScene::PauseScene(const AppServices& services)
    : services_{services}
{
}

void PauseScene::update(sgl::SceneContext& ctx, sgl::Seconds)
{
    const sgl::InputState& input = ctx.input();
    const Actions& actions = services_.actions;

    if(input.action(actions.pause).pressed || input.action(actions.cancel).pressed)
    {
        ctx.request(sgl::PopScene{});
        return;
    }

    if(input.action(actions.confirm).pressed)
    {
        ctx.request(sgl::PopScene{});
        ctx.request(sgl::ReplaceScene{mainMenu(services_)});
    }
}

void PauseScene::render(sgl::RenderQueue& queue) const
{
    queue.push(sgl::Layer::Overlay,
               0.f,
               sgl::RectCmd{
                   .rect = {.pos = {0.f, 0.f}, .size = {designWidth, designHeight}},
                   .fill = sgl::Color{0, 0, 0, 150},
               });
    queue.push(sgl::Layer::Overlay,
               1.f,
               sgl::RectCmd{
                   .rect = {.pos = {380.f, 240.f}, .size = {520.f, 240.f}},
                   .fill = sgl::Color{24, 28, 48, 230},
               });
    queue.push(sgl::Layer::Overlay,
               2.f,
               sgl::TextCmd{
                   .font = services_.font,
                   .text = "Paused",
                   .size = 42,
                   .position = {designWidth * 0.5f, 300.f},
                   .color = titleGold,
                   .anchor = sgl::Anchor::Center,
               });
    queue.push(sgl::Layer::Overlay,
               3.f,
               sgl::TextCmd{
                   .font = services_.font,
                   .text = "Esc - resume",
                   .size = 22,
                   .position = {designWidth * 0.5f, 360.f},
                   .color = sgl::Color{255, 255, 255},
                   .anchor = sgl::Anchor::Center,
               });
    queue.push(sgl::Layer::Overlay,
               4.f,
               sgl::TextCmd{
                   .font = services_.font,
                   .text = "Enter - quit to menu",
                   .size = 22,
                   .position = {designWidth * 0.5f, 400.f},
                   .color = sgl::Color{255, 255, 255},
                   .anchor = sgl::Anchor::Center,
               });
}

sgl::SceneTraits PauseScene::traits() const
{
    return sgl::SceneTraits{.opaque = false, .blocksUpdate = true, .pausable = false};
}

} // namespace sgl::arkanoid
