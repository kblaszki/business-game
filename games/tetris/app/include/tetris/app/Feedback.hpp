#pragma once

#include <cstddef>
#include <cstdint>
#include <sgl/audio/AudioI.hpp>
#include <sgl/core/Random.hpp>
#include <sgl/core/Time.hpp>
#include <sgl/fx/ParticleSystem.hpp>
#include <sgl/render/RenderQueue.hpp>
#include <span>
#include <tetris/app/Sounds.hpp>
#include <tetris/sim/Events.hpp>

namespace sgl::tetris
{

class Feedback
{
public:
    static constexpr std::size_t kParticleCapacity{512};
    static constexpr std::size_t kParticlesPerRow{12};

    Feedback(sgl::AudioI& audio, SoundIds sounds, sgl::Pcg32 seed);

    void onEvents(std::span<const TetrisEvent> events);
    void update(sgl::Seconds dt);
    void render(sgl::RenderQueue& queue) const;

    [[nodiscard]] std::size_t particleCount() const noexcept;

private:
    void playLineSound(std::uint8_t count);
    void emitRowBurst(std::uint8_t rowIndex);

    sgl::AudioI& audio_;
    SoundIds sounds_{};
    sgl::Pcg32 rng_;
    sgl::ParticleSystem particles_;
};

} // namespace sgl::tetris
