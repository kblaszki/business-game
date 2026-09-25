#include <sgl/render/DrawCommand.hpp>
#include <sgl/render/RenderQueue.hpp>
#include <sgl/scene/SceneContext.hpp>
#include <string>
#include <tetris/app/GameOverScene.hpp>
#include <tetris/app/Theme.hpp>

namespace sgl::tetris
{
namespace
{

constexpr sgl::Vec2f kButtonSize{360.f, 64.f};
constexpr float kRetryY{360.f};
constexpr float kMenuY{440.f};

[[nodiscard]] sgl::Rect<float> centeredButton(float y)
{
    return sgl::Rect<float>{
        .pos = {(designWidth - kButtonSize.x) * 0.5f, y},
        .size = kButtonSize,
    };
}

} // namespace

GameOverScene::GameOverScene(const AppServices& services, std::uint32_t score)
    : services_{services}
    , score_{score}
    , retryButton_{centeredButton(kRetryY)}
    , menuButton_{centeredButton(kMenuY)}
{
}

std::uint32_t GameOverScene::score() const noexcept
{
    return score_;
}

bool GameOverScene::hitRetry(sgl::Vec2f point) const
{
    return retryButton_.contains(point);
}

bool GameOverScene::hitMenu(sgl::Vec2f point) const
{
    return menuButton_.contains(point);
}

void GameOverScene::update(sgl::SceneContext& ctx, sgl::Seconds)
{
    const sgl::InputState& input = ctx.input();

    if(const auto pointer = input.pointer())
    {
        retryHover_ = hitRetry(*pointer);
        menuHover_ = hitMenu(*pointer);

        if(input.pointerPressed())
        {
            if(retryHover_)
            {
                ctx.request(sgl::PopScene{});
                ctx.request(sgl::ReplaceScene{play(services_)});
                return;
            }
            if(menuHover_)
            {
                ctx.request(sgl::PopScene{});
                ctx.request(sgl::ReplaceScene{mainMenu(services_)});
                return;
            }
        }
    }
    else
    {
        retryHover_ = false;
        menuHover_ = false;
    }

    if(input.action(services_.actions.confirm).pressed)
    {
        ctx.request(sgl::PopScene{});
        ctx.request(sgl::ReplaceScene{play(services_)});
        return;
    }

    if(input.action(services_.actions.cancel).pressed)
    {
        ctx.request(sgl::PopScene{});
        ctx.request(sgl::ReplaceScene{mainMenu(services_)});
    }
}

void GameOverScene::render(sgl::RenderQueue& queue) const
{
    queue.push(sgl::Layer::Overlay,
               0.f,
               sgl::RectCmd{
                   .rect = {.pos = {0.f, 0.f}, .size = {designWidth, designHeight}},
                   .fill = overlayDim,
               });
    queue.push(sgl::Layer::Overlay,
               1.f,
               sgl::RectCmd{
                   .rect = {.pos = {340.f, 160.f}, .size = {600.f, 400.f}},
                   .fill = panelBox,
               });
    queue.push(sgl::Layer::Overlay,
               2.f,
               sgl::TextCmd{
                   .font = services_.font,
                   .text = "Game Over",
                   .size = 42,
                   .position = {designWidth * 0.5f, 220.f},
                   .color = titleGold,
                   .anchor = sgl::Anchor::Center,
               });
    queue.push(sgl::Layer::Overlay,
               3.f,
               sgl::TextCmd{
                   .font = services_.font,
                   .text = "Score " + std::to_string(score_),
                   .size = 28,
                   .position = {designWidth * 0.5f, 280.f},
                   .color = hudText,
                   .anchor = sgl::Anchor::Center,
               });

    queue.push(sgl::Layer::Overlay,
               4.f,
               sgl::RectCmd{
                   .rect = retryButton_,
                   .fill = retryHover_ ? startButtonHover : startButton,
               });
    queue.push(sgl::Layer::Overlay,
               5.f,
               sgl::TextCmd{
                   .font = services_.font,
                   .text = "Retry",
                   .size = 26,
                   .position = {retryButton_.pos.x + retryButton_.size.x * 0.5f,
                                retryButton_.pos.y + retryButton_.size.y * 0.5f},
                   .color = startLabel,
                   .anchor = sgl::Anchor::Center,
               });

    queue.push(sgl::Layer::Overlay,
               6.f,
               sgl::RectCmd{
                   .rect = menuButton_,
                   .fill = menuHover_ ? quitButtonHover : quitButton,
               });
    queue.push(
        sgl::Layer::Overlay,
        7.f,
        sgl::TextCmd{
            .font = services_.font,
            .text = "Menu",
            .size = 26,
            .position = {menuButton_.pos.x + menuButton_.size.x * 0.5f, menuButton_.pos.y + menuButton_.size.y * 0.5f},
            .color = quitLabel,
            .anchor = sgl::Anchor::Center,
        });
}

sgl::SceneTraits GameOverScene::traits() const
{
    return sgl::SceneTraits{.opaque = true, .blocksUpdate = true, .pausable = false};
}

} // namespace sgl::tetris
