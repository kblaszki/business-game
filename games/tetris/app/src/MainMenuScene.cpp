#include <sgl/render/DrawCommand.hpp>
#include <sgl/render/RenderQueue.hpp>
#include <sgl/scene/SceneContext.hpp>
#include <tetris/app/MainMenuScene.hpp>
#include <tetris/app/Theme.hpp>

namespace sgl::tetris
{
namespace
{

constexpr sgl::Vec2f kButtonSize{400.f, 72.f};
constexpr float kStartY{320.f};
constexpr float kQuitY{420.f};
constexpr float kTitleY{200.f};

[[nodiscard]] sgl::Rect<float> centeredButton(float y)
{
    return sgl::Rect<float>{
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

bool MainMenuScene::hitStart(sgl::Vec2f point) const
{
    return startButton_.contains(point);
}

bool MainMenuScene::hitQuit(sgl::Vec2f point) const
{
    return quitButton_.contains(point);
}

void MainMenuScene::update(sgl::SceneContext& ctx, sgl::Seconds)
{
    const sgl::InputState& input = ctx.input();

    if(const auto pointer = input.pointer())
    {
        startHover_ = hitStart(*pointer);
        quitHover_ = hitQuit(*pointer);

        if(input.pointerPressed())
        {
            if(startHover_)
            {
                ctx.request(sgl::ReplaceScene{play(services_)});
                return;
            }
            if(quitHover_)
            {
                ctx.request(sgl::QuitApp{});
                return;
            }
        }
    }
    else
    {
        startHover_ = false;
        quitHover_ = false;
    }

    if(input.action(services_.actions.confirm).pressed)
    {
        ctx.request(sgl::ReplaceScene{play(services_)});
        return;
    }

    if(input.action(services_.actions.cancel).pressed)
    {
        ctx.request(sgl::QuitApp{});
    }
}

void MainMenuScene::render(sgl::RenderQueue& queue) const
{
    queue.push(sgl::Layer::Hud,
               0.f,
               sgl::TextCmd{
                   .font = services_.font,
                   .text = "Tetris",
                   .size = 56,
                   .position = {designWidth * 0.5f, kTitleY},
                   .color = titleGold,
                   .anchor = sgl::Anchor::Center,
               });

    queue.push(sgl::Layer::Hud,
               1.f,
               sgl::RectCmd{
                   .rect = startButton_,
                   .fill = startHover_ ? startButtonHover : startButton,
               });
    queue.push(sgl::Layer::Hud,
               2.f,
               sgl::TextCmd{
                   .font = services_.font,
                   .text = "Start",
                   .size = 28,
                   .position = {startButton_.pos.x + startButton_.size.x * 0.5f,
                                startButton_.pos.y + startButton_.size.y * 0.5f},
                   .color = startLabel,
                   .anchor = sgl::Anchor::Center,
               });

    queue.push(sgl::Layer::Hud,
               3.f,
               sgl::RectCmd{
                   .rect = quitButton_,
                   .fill = quitHover_ ? quitButtonHover : quitButton,
               });
    queue.push(
        sgl::Layer::Hud,
        4.f,
        sgl::TextCmd{
            .font = services_.font,
            .text = "Quit",
            .size = 28,
            .position = {quitButton_.pos.x + quitButton_.size.x * 0.5f, quitButton_.pos.y + quitButton_.size.y * 0.5f},
            .color = quitLabel,
            .anchor = sgl::Anchor::Center,
        });
}

sgl::SceneTraits MainMenuScene::traits() const
{
    return sgl::SceneTraits{.opaque = true, .blocksUpdate = true, .pausable = false};
}

} // namespace sgl::tetris
