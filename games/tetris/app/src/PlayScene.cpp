#include <sgl/render/DrawCommand.hpp>
#include <sgl/render/RenderQueue.hpp>
#include <sgl/scene/SceneContext.hpp>
#include <tetris/app/InputMapping.hpp>
#include <tetris/app/PlayScene.hpp>
#include <tetris/app/Theme.hpp>

namespace sgl::tetris
{

PlayScene::PlayScene(const AppServices& services)
    : services_{services}
    , layout_{defaultLayout()}
    , game_{services.seed}
{
}

void PlayScene::update(sgl::SceneContext& ctx, sgl::Seconds dt)
{
    const sgl::InputState& input = ctx.input();
    const Actions& actions = services_.actions;

    if(game_.over())
    {
        return;
    }

    if(input.action(actions.pause).pressed)
    {
        ctx.request(sgl::RequestPause{});
        return;
    }

    game_.step(toTetrisInput(input, actions), dt);

    if(game_.over())
    {
        ctx.request(sgl::PushScene{gameOver(services_, game_.score().points)});
    }
}

void PlayScene::render(sgl::RenderQueue& queue) const
{
    renderBoard(game_, layout_, queue);

    const HudModel model = hud();
    queue.push(sgl::Layer::Hud,
               10.f,
               sgl::TextCmd{
                   .font = services_.font,
                   .text = model.score,
                   .size = 22,
                   .position = hudScorePos,
                   .color = hudText,
               });
    queue.push(sgl::Layer::Hud,
               11.f,
               sgl::TextCmd{
                   .font = services_.font,
                   .text = model.lines,
                   .size = 22,
                   .position = hudLinesPos,
                   .color = hudText,
               });
    queue.push(sgl::Layer::Hud,
               12.f,
               sgl::TextCmd{
                   .font = services_.font,
                   .text = model.level,
                   .size = 22,
                   .position = hudLevelPos,
                   .color = hudText,
               });
}

sgl::SceneTraits PlayScene::traits() const
{
    return sgl::SceneTraits{.opaque = true, .blocksUpdate = true, .pausable = !game_.over()};
}

TetrisGame& PlayScene::game()
{
    return game_;
}

const TetrisGame& PlayScene::game() const
{
    return game_;
}

HudModel PlayScene::hud() const
{
    return makeHud(game_.score());
}

} // namespace sgl::tetris
