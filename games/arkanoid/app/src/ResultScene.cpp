#include <arkanoid/app/ResultScene.hpp>
#include <arkanoid/app/Theme.hpp>
#include <arkanoid/sim/Tuning.hpp>
#include <sgl/render/DrawCommand.hpp>
#include <sgl/render/RenderQueue.hpp>
#include <sgl/scene/SceneContext.hpp>
#include <string>

namespace sgl::arkanoid
{
namespace
{

constexpr sgl::Vec2f kButtonSize{360.f, 64.f};
constexpr float kRetryY{400.f};
constexpr float kMenuY{480.f};

[[nodiscard]] sgl::Rect<float> centeredButton(float y)
{
    return sgl::Rect<float>{
        .pos = {(designWidth - kButtonSize.x) * 0.5f, y},
        .size = kButtonSize,
    };
}

} // namespace

ResultScene::ResultScene(
    const AppServices& services, Outcome outcome, std::uint32_t score, std::optional<std::size_t> rank, bool saveFailed)
    : services_{services}
    , outcome_{outcome}
    , score_{score}
    , rank_{rank}
    , saveFailed_{saveFailed}
    , retryButton_{centeredButton(kRetryY)}
    , menuButton_{centeredButton(kMenuY)}
{
}

Outcome ResultScene::outcome() const noexcept
{
    return outcome_;
}

std::uint32_t ResultScene::score() const noexcept
{
    return score_;
}

std::optional<std::size_t> ResultScene::rank() const noexcept
{
    return rank_;
}

bool ResultScene::saveFailed() const noexcept
{
    return saveFailed_;
}

bool ResultScene::hitRetry(sgl::Vec2f point) const
{
    return retryButton_.contains(point);
}

bool ResultScene::hitMenu(sgl::Vec2f point) const
{
    return menuButton_.contains(point);
}

void ResultScene::update(sgl::SceneContext& ctx, sgl::Seconds)
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
                ctx.request(sgl::ReplaceScene{gameplay(services_, StageId::Stage1)});
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
        ctx.request(sgl::ReplaceScene{gameplay(services_, StageId::Stage1)});
        return;
    }

    if(input.action(services_.actions.cancel).pressed)
    {
        ctx.request(sgl::PopScene{});
        ctx.request(sgl::ReplaceScene{mainMenu(services_)});
    }
}

void ResultScene::render(sgl::RenderQueue& queue) const
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
                   .rect = {.pos = {340.f, 140.f}, .size = {600.f, 440.f}},
                   .fill = sgl::Color{24, 28, 48, 230},
               });

    const char* title = outcome_ == Outcome::Won ? "You win" : "You lose";
    queue.push(sgl::Layer::Overlay,
               2.f,
               sgl::TextCmd{
                   .font = services_.font,
                   .text = title,
                   .size = 42,
                   .position = {designWidth * 0.5f, 200.f},
                   .color = titleGold,
                   .anchor = sgl::Anchor::Center,
               });
    queue.push(sgl::Layer::Overlay,
               3.f,
               sgl::TextCmd{
                   .font = services_.font,
                   .text = "Score " + std::to_string(score_),
                   .size = 28,
                   .position = {designWidth * 0.5f, 260.f},
                   .color = hudText,
                   .anchor = sgl::Anchor::Center,
               });

    float nextY = 310.f;
    if(rank_.has_value())
    {
        queue.push(sgl::Layer::Overlay,
                   4.f,
                   sgl::TextCmd{
                       .font = services_.font,
                       .text = "New record",
                       .size = 24,
                       .position = {designWidth * 0.5f, nextY},
                       .color = bannerTitle,
                       .anchor = sgl::Anchor::Center,
                   });
        nextY += 40.f;
    }
    if(saveFailed_)
    {
        queue.push(sgl::Layer::Overlay,
                   5.f,
                   sgl::TextCmd{
                       .font = services_.font,
                       .text = "Could not save high score",
                       .size = 20,
                       .position = {designWidth * 0.5f, nextY},
                       .color = bannerHint,
                       .anchor = sgl::Anchor::Center,
                   });
    }

    queue.push(sgl::Layer::Overlay,
               6.f,
               sgl::RectCmd{
                   .rect = retryButton_,
                   .fill = retryHover_ ? startButtonHover : startButton,
               });
    queue.push(sgl::Layer::Overlay,
               7.f,
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
               8.f,
               sgl::RectCmd{
                   .rect = menuButton_,
                   .fill = menuHover_ ? quitButtonHover : quitButton,
               });
    queue.push(
        sgl::Layer::Overlay,
        9.f,
        sgl::TextCmd{
            .font = services_.font,
            .text = "Menu",
            .size = 26,
            .position = {menuButton_.pos.x + menuButton_.size.x * 0.5f, menuButton_.pos.y + menuButton_.size.y * 0.5f},
            .color = quitLabel,
            .anchor = sgl::Anchor::Center,
        });
}

sgl::SceneTraits ResultScene::traits() const
{
    return sgl::SceneTraits{.opaque = false, .blocksUpdate = true, .pausable = false};
}

} // namespace sgl::arkanoid
