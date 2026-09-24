#include <arkanoid/app/SceneRender.hpp>

#include <arkanoid/sim/Tuning.hpp>
#include <eng/render/DrawCommand.hpp>

#include <cstddef>

namespace arkanoid
{

void renderState(const State& state, const TextureIds& textures, eng::RenderQueue& queue)
{
    queue.push(eng::Layer::Background, 0.f,
               eng::SpriteCmd{
                   .texture = textures.background,
                   .source = {.pos = {0.f, 0.f}, .size = {designWidth, designHeight}},
                   .position = {0.f, 0.f},
               });

    for(const Brick& brick : state.bricks)
    {
        if(!brick.alive)
        {
            continue;
        }
        queue.push(eng::Layer::World, brick.box.pos.y,
                   eng::SpriteCmd{
                       .texture = textures.brick,
                       .source = {.pos = {0.f, 0.f}, .size = {brickWidth, brickHeight}},
                       .position = brick.box.pos,
                       .scale = {1.f, 1.f},
                       .tint = brick.tint,
                   });
    }

    queue.push(eng::Layer::Actors, paddleY,
               eng::SpriteCmd{
                   .texture = textures.paddle,
                   .source = {.pos = {0.f, 0.f}, .size = {paddleWidth, paddleHeight}},
                   .position = {state.paddle.x, paddleY},
                   .scale = {state.paddle.width / paddleWidth, 1.f},
               });

    for(const Ball& ball : state.balls)
    {
        queue.push(eng::Layer::Actors, ball.pos.y,
                   eng::SpriteCmd{
                       .texture = textures.ball,
                       .source = {.pos = {0.f, 0.f}, .size = {ballDiameter, ballDiameter}},
                       .position = ball.pos,
                   });
    }

    for(const Capsule& capsule : state.capsules)
    {
        queue.push(eng::Layer::Actors, capsule.pos.y,
                   eng::SpriteCmd{
                       .texture = textures.capsule[static_cast<std::size_t>(capsule.kind)],
                       .source = {.pos = {0.f, 0.f}, .size = {capsuleWidth, capsuleHeight}},
                       .position = capsule.pos,
                   });
    }
}

} // namespace arkanoid
