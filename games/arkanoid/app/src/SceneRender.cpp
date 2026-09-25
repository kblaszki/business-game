#include <arkanoid/app/SceneRender.hpp>
#include <arkanoid/sim/Tuning.hpp>
#include <cstddef>
#include <sgl/render/DrawCommand.hpp>

namespace sgl::arkanoid
{

void renderState(const State& state, const TextureIds& textures, sgl::RenderQueue& queue)
{
    queue.push(sgl::Layer::Background,
               0.f,
               sgl::SpriteCmd{
                   .texture = textures.background,
                   .source = {.pos = {0.f, 0.f}, .size = {designWidth, designHeight}},
                   .position = {0.f, 0.f},
               });

    for(const Brick& brick: state.bricks)
    {
        if(!brick.alive)
        {
            continue;
        }
        queue.push(sgl::Layer::World,
                   brick.box.pos.y,
                   sgl::SpriteCmd{
                       .texture = textures.brick,
                       .source = {.pos = {0.f, 0.f}, .size = {brickWidth, brickHeight}},
                       .position = brick.box.pos,
                       .scale = {1.f, 1.f},
                       .tint = brick.tint,
                   });
    }

    queue.push(sgl::Layer::Actors,
               paddleY,
               sgl::SpriteCmd{
                   .texture = textures.paddle,
                   .source = {.pos = {0.f, 0.f}, .size = {paddleNormalWidth, paddleHeight}},
                   .position = {state.paddle.x, paddleY},
                   .scale = {state.paddle.width / paddleNormalWidth, 1.f},
               });

    for(const Ball& ball: state.balls)
    {
        queue.push(sgl::Layer::Actors,
                   ball.pos.y,
                   sgl::SpriteCmd{
                       .texture = textures.ball,
                       .source = {.pos = {0.f, 0.f}, .size = {ballDiameter, ballDiameter}},
                       .position = ball.pos,
                   });
    }

    for(const Capsule& capsule: state.capsules)
    {
        queue.push(sgl::Layer::Actors,
                   capsule.pos.y,
                   sgl::SpriteCmd{
                       .texture = textures.capsule[static_cast<std::size_t>(capsule.kind)],
                       .source = {.pos = {0.f, 0.f}, .size = {capsuleWidth, capsuleHeight}},
                       .position = capsule.pos,
                   });
    }
}

} // namespace sgl::arkanoid
