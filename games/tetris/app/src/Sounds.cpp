#include <array>
#include <sgl/audio/Envelope.hpp>
#include <sgl/audio/Tone.hpp>
#include <sgl/audio/Waveform.hpp>
#include <sgl/core/Time.hpp>
#include <tetris/app/Sounds.hpp>

namespace sgl::tetris
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
        {.key = "move", .pcm = blip(sgl::Waveform::Square, 440.f, 0.04f)},
        {.key = "rotate", .pcm = blip(sgl::Waveform::Square, 660.f, 0.05f)},
        {.key = "lock", .pcm = blip(sgl::Waveform::Triangle, 220.f, 0.08f)},
        {.key = "line1", .pcm = blip(sgl::Waveform::Square, 523.f, 0.1f)},
        {.key = "line2", .pcm = blip(sgl::Waveform::Square, 587.f, 0.12f)},
        {.key = "line3", .pcm = blip(sgl::Waveform::Square, 659.f, 0.14f)},
        {.key = "tetris", .pcm = blip(sgl::Waveform::Square, 784.f, 0.22f)},
        {.key = "levelUp", .pcm = blip(sgl::Waveform::Triangle, 880.f, 0.16f)},
        {.key = "gameOver", .pcm = blip(sgl::Waveform::Triangle, 110.f, 0.35f)},
    }};
    return specs;
}

} // namespace sgl::tetris
