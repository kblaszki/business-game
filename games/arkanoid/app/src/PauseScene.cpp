#include <arkanoid/app/PauseScene.hpp>

#include <arkanoid/app/Theme.hpp>
#include <arkanoid/sim/Tuning.hpp>
#include <eng/render/DrawCommand.hpp>
#include <eng/render/RenderQueue.hpp>
#include <eng/scene/SceneContext.hpp>

namespace arkanoid
{

PauseScene::PauseScene(const AppServices& services)
    : services_{services}
{
}

void PauseScene::update(eng::SceneContext& ctx, eng::Seconds)
{
    const eng::InputState& input = ctx.input();
    const Actions& actions = services_.actions;

    if(input.action(actions.pause).pressed || input.action(actions.cancel).pressed)
    {
        ctx.request(eng::PopScene{});
        return;
    }

    if(input.action(actions.confirm).pressed)
    {
        ctx.request(eng::PopScene{});
        ctx.request(eng::ReplaceScene{mainMenu(services_)});
    }
}

void PauseScene::render(eng::RenderQueue& queue) const
{
    queue.push(eng::Layer::Overlay, 0.f,
               eng::RectCmd{
                   .rect = {.pos = {0.f, 0.f}, .size = {designWidth, designHeight}},
                   .fill = eng::Color{0, 0, 0, 150},
               });
    queue.push(eng::Layer::Overlay, 1.f,
               eng::RectCmd{
                   .rect = {.pos = {380.f, 240.f}, .size = {520.f, 240.f}},
                   .fill = eng::Color{24, 28, 48, 230},
               });
    queue.push(eng::Layer::Overlay, 2.f,
               eng::TextCmd{
                   .font = services_.font,
                   .text = "Paused",
                   .size = 42,
                   .position = {designWidth * 0.5f, 300.f},
                   .color = titleGold,
                   .anchor = eng::Anchor::Center,
               });
    queue.push(eng::Layer::Overlay, 3.f,
               eng::TextCmd{
                   .font = services_.font,
                   .text = "Esc - resume",
                   .size = 22,
                   .position = {designWidth * 0.5f, 360.f},
                   .color = eng::Color{255, 255, 255},
                   .anchor = eng::Anchor::Center,
               });
    queue.push(eng::Layer::Overlay, 4.f,
               eng::TextCmd{
                   .font = services_.font,
                   .text = "Enter - quit to menu",
                   .size = 22,
                   .position = {designWidth * 0.5f, 400.f},
                   .color = eng::Color{255, 255, 255},
                   .anchor = eng::Anchor::Center,
               });
}

eng::SceneTraits PauseScene::traits() const
{
    return eng::SceneTraits{.opaque = false, .blocksUpdate = true, .pausable = false};
}

} // namespace arkanoid
