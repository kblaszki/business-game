#include <arkanoid/app/Sounds.hpp>
#include <array>
#include <sgl/audio/Envelope.hpp>
#include <sgl/audio/Tone.hpp>
#include <sgl/audio/Waveform.hpp>
#include <sgl/core/Time.hpp>

namespace sgl::arkanoid
{
namespace
{

constexpr std::uint32_t kSampleRate{22050};
constexpr float kAmp{0.35f};

[[nodiscard]] sgl::Pcm blip(sgl::Waveform wave, float hz, float seconds)
{
    return sgl::tone(wave,
                     hz,
                     sgl::Seconds{seconds},
                     kSampleRate,
                     sgl::Envelope{.attack = sgl::Seconds{0.005f}, .release = sgl::Seconds{0.03f}},
                     kAmp);
}

} // namespace

std::span<const SoundSpec> soundSpecs()
{
    static const std::array<SoundSpec, 9> specs{{
        {.key = "paddle", .pcm = blip(sgl::Waveform::Square, 330.f, 0.05f)},
        {.key = "brick", .pcm = blip(sgl::Waveform::Square, 520.f, 0.06f)},
        {.key = "wall", .pcm = blip(sgl::Waveform::Triangle, 220.f, 0.04f)},
        {.key = "launch", .pcm = blip(sgl::Waveform::Square, 440.f, 0.08f)},
        {.key = "powerUp", .pcm = blip(sgl::Waveform::Triangle, 660.f, 0.12f)},
        {.key = "ballLost", .pcm = blip(sgl::Waveform::Triangle, 180.f, 0.1f)},
        {.key = "lifeLost", .pcm = blip(sgl::Waveform::Triangle, 140.f, 0.18f)},
        {.key = "stageClear", .pcm = blip(sgl::Waveform::Square, 784.f, 0.22f)},
        {.key = "gameOver", .pcm = blip(sgl::Waveform::Triangle, 110.f, 0.35f)},
    }};
    return specs;
}

} // namespace sgl::arkanoid
