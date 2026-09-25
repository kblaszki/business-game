#pragma once

#include <arkanoid/app/Sounds.hpp>
#include <arkanoid/sim/Events.hpp>
#include <arkanoid/sim/State.hpp>
#include <cstddef>
#include <sgl/audio/AudioI.hpp>
#include <sgl/core/Random.hpp>
#include <sgl/core/Time.hpp>
#include <sgl/fx/ParticleSystem.hpp>
#include <sgl/render/RenderQueue.hpp>
#include <span>

namespace sgl::arkanoid
{

class Feedback
{
public:
    static constexpr std::size_t kParticleCapacity{1024};
    static constexpr std::size_t kParticlesPerBrick{12};

    Feedback(sgl::AudioI& audio, SoundIds sounds, sgl::Pcg32 seed);

    void onEvents(std::span<const SimEvent> events, const State& state);
    void update(sgl::Seconds dt);
    void render(sgl::RenderQueue& queue) const;

    [[nodiscard]] std::size_t particleCount() const noexcept;

private:
    void emitBrickBurst(const Brick& brick);

    sgl::AudioI& audio_;
    SoundIds sounds_{};
    sgl::Pcg32 rng_;
    sgl::ParticleSystem particles_;
};

} // namespace sgl::arkanoid
