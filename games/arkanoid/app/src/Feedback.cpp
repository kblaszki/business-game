#include <arkanoid/app/Feedback.hpp>
#include <sgl/core/Overloaded.hpp>
#include <variant>

namespace sgl::arkanoid
{

Feedback::Feedback(sgl::AudioI& audio, SoundIds sounds, sgl::Pcg32 seed)
    : audio_{audio}
    , sounds_{sounds}
    , rng_{seed}
    , particles_{kParticleCapacity}
{
}

void Feedback::onEvents(std::span<const SimEvent> events, const State& state)
{
    for(const SimEvent& event: events)
    {
        std::visit(sgl::Overloaded{
                       [this, &state](const BrickDestroyed& destroyed) {
                           audio_.play(sounds_.brick, 1.f);
                           if(destroyed.index < state.bricks.size())
                           {
                               emitBrickBurst(state.bricks[destroyed.index]);
                           }
                       },
                       [this](const LifeLost&) { audio_.play(sounds_.lifeLost, 1.f); },
                       [this](const StageCleared&) { audio_.play(sounds_.stageClear, 1.f); },
                       [this](const GameOver&) { audio_.play(sounds_.gameOver, 1.f); },
                       [this](const PowerUpCaught&) { audio_.play(sounds_.powerUp, 1.f); },
                       [this](const PaddleHit&) { audio_.play(sounds_.paddle, 1.f); },
                       [this](const WallHit&) { audio_.play(sounds_.wall, 1.f); },
                       [this](const BallLaunched&) { audio_.play(sounds_.launch, 1.f); },
                       [this](const BallLost&) { audio_.play(sounds_.ballLost, 1.f); },
                   },
                   event);
    }
}

void Feedback::update(sgl::Seconds dt)
{
    particles_.update(dt);
}

void Feedback::render(sgl::RenderQueue& queue) const
{
    particles_.render(queue, sgl::Layer::Overlay);
}

std::size_t Feedback::particleCount() const noexcept
{
    return particles_.size();
}

void Feedback::emitBrickBurst(const Brick& brick)
{
    particles_.emit(
        sgl::BurstParams{
            .origin = {brick.box.pos.x + brick.box.size.x * 0.5f, brick.box.pos.y + brick.box.size.y * 0.5f},
            .count = kParticlesPerBrick,
            .speedMin = 40.f,
            .speedMax = 160.f,
            .angleMinDeg = 0.f,
            .angleMaxDeg = 360.f,
            .lifeMin = 0.35f,
            .lifeMax = 0.7f,
            .size = 4.f,
            .color = brick.tint,
            .gravity = {0.f, 280.f},
        },
        rng_);
}

} // namespace sgl::arkanoid
