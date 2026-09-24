#include <arkanoid/app/MainMenuScene.hpp>

#include <arkanoid/app/Theme.hpp>
#include <arkanoid/sim/Tuning.hpp>
#include <eng/render/DrawCommand.hpp>
#include <eng/render/RenderQueue.hpp>
#include <eng/scene/SceneContext.hpp>

namespace arkanoid
{
namespace
{

constexpr eng::Vec2f kButtonSize{400.f, 72.f};
constexpr float kStartY{320.f};
constexpr float kQuitY{420.f};
constexpr float kTitleY{200.f};

[[nodiscard]] eng::Rect<float> centeredButton(float y)
{
    return eng::Rect<float>{
        .pos = {(designWidth - kButtonSize.x) * 0.5f, y},
        .size = kButtonSize,
    };
}

} // namespace

MainMenuScene::MainMenuScene(const AppServices& services)
    : services_{services}
    , startButton_{centeredButton(kStartY)}
    , quitButton_{centeredButton(kQuitY)}
{
}

bool MainMenuScene::hitStart(eng::Vec2f point) const
{
    return startButton_.contains(point);
}

bool MainMenuScene::hitQuit(eng::Vec2f point) const
{
    return quitButton_.contains(point);
}

void MainMenuScene::update(eng::SceneContext& ctx, eng::Seconds)
{
    const eng::InputState& input = ctx.input();

    if(const auto pointer = input.pointer())
    {
        startHover_ = hitStart(*pointer);
        quitHover_ = hitQuit(*pointer);

        // InputState keeps the last pointer from MouseDown/MouseMove; treat a hit as a click.
        if(startHover_)
        {
            ctx.request(eng::ReplaceScene{gameplay(services_, StageId::Stage1)});
            return;
        }
        if(quitHover_)
        {
            ctx.request(eng::QuitApp{});
            return;
        }
    }
    else
    {
        startHover_ = false;
        quitHover_ = false;
    }

    if(input.action(services_.actions.confirm).pressed)
    {
        ctx.request(eng::ReplaceScene{gameplay(services_, StageId::Stage1)});
        return;
    }

    if(input.action(services_.actions.cancel).pressed)
    {
        ctx.request(eng::QuitApp{});
    }
}

void MainMenuScene::render(eng::RenderQueue& queue) const
{
    queue.push(eng::Layer::Hud, 0.f,
               eng::TextCmd{
                   .font = services_.font,
                   .text = "Breakout",
                   .size = 56,
                   .position = {designWidth * 0.5f, kTitleY},
                   .color = titleGold,
                   .anchor = eng::Anchor::Center,
               });

    queue.push(eng::Layer::Hud, 1.f,
               eng::RectCmd{
                   .rect = startButton_,
                   .fill = startHover_ ? startButtonHover : startButton,
               });
    queue.push(eng::Layer::Hud, 2.f,
               eng::TextCmd{
                   .font = services_.font,
                   .text = "Start",
                   .size = 28,
                   .position = {startButton_.pos.x + startButton_.size.x * 0.5f,
                                startButton_.pos.y + startButton_.size.y * 0.5f},
                   .color = startLabel,
                   .anchor = eng::Anchor::Center,
               });

    queue.push(eng::Layer::Hud, 3.f,
               eng::RectCmd{
                   .rect = quitButton_,
                   .fill = quitHover_ ? quitButtonHover : quitButton,
               });
    queue.push(eng::Layer::Hud, 4.f,
               eng::TextCmd{
                   .font = services_.font,
                   .text = "Quit",
                   .size = 28,
                   .position = {quitButton_.pos.x + quitButton_.size.x * 0.5f,
                                quitButton_.pos.y + quitButton_.size.y * 0.5f},
                   .color = quitLabel,
                   .anchor = eng::Anchor::Center,
               });
}

eng::SceneTraits MainMenuScene::traits() const
{
    return eng::SceneTraits{.opaque = true, .blocksUpdate = true, .pausable = false};
}

} // namespace arkanoid
