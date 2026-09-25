#include <algorithm>
#include <cmath>
#include <cstdint>
#include <sgl/audio/Tone.hpp>
#include <sgl/core/Random.hpp>

namespace sgl
{
namespace
{

[[nodiscard]] float sampleWave(Waveform waveform, float phase, Pcg32& rng) noexcept
{
    switch(waveform)
    {
        case Waveform::Square:
            return phase < 0.5f ? 1.f : -1.f;
        case Waveform::Triangle:
            if(phase < 0.5f)
            {
                return phase * 4.f - 1.f;
            }
            return 3.f - phase * 4.f;
        case Waveform::Noise:
            return rng.uniformFloat(-1.f, 1.f);
    }
    return 0.f;
}

[[nodiscard]] float
envelopeGain(std::size_t index, std::size_t count, std::size_t attackN, std::size_t releaseN) noexcept
{
    if(count == 0)
    {
        return 0.f;
    }

    if(attackN + releaseN > count)
    {
        const float mid = static_cast<float>(count) * 0.5f;
        attackN = static_cast<std::size_t>(mid);
        releaseN = count - attackN;
    }

    if(index < attackN)
    {
        if(attackN == 0)
        {
            return 1.f;
        }
        return static_cast<float>(index) / static_cast<float>(attackN);
    }

    if(index >= count - releaseN)
    {
        if(releaseN == 0)
        {
            return 1.f;
        }
        const std::size_t fromEnd = count - 1 - index;
        return static_cast<float>(fromEnd) / static_cast<float>(releaseN);
    }

    return 1.f;
}

} // namespace

Pcm tone(
    Waveform waveform, float freqHz, Seconds duration, std::uint32_t sampleRate, Envelope envelope, float amplitude)
{
    Pcm pcm{.sampleRate = sampleRate, .samples = {}};

    if(sampleRate == 0 || duration.count() <= 0.f)
    {
        return pcm;
    }

    const auto count = static_cast<std::size_t>(duration.count() * static_cast<float>(sampleRate));
    pcm.samples.resize(count);

    const auto attackN =
        static_cast<std::size_t>(std::max(0.f, envelope.attack.count()) * static_cast<float>(sampleRate));
    const auto releaseN =
        static_cast<std::size_t>(std::max(0.f, envelope.release.count()) * static_cast<float>(sampleRate));

    Pcg32 rng{0xC2B2AE35u};
    const float invRate = 1.f / static_cast<float>(sampleRate);
    float phase = 0.f;
    const float phaseStep = (freqHz > 0.f) ? (freqHz * invRate) : 0.f;

    for(std::size_t i = 0; i < count; ++i)
    {
        const float wave = sampleWave(waveform, phase, rng);
        const float gain = envelopeGain(i, count, attackN, releaseN);
        const float sample = wave * amplitude * gain;
        const float clamped = std::clamp(sample, -1.f, 1.f);
        pcm.samples[i] = static_cast<std::int16_t>(clamped * 32767.f);

        phase += phaseStep;
        if(phase >= 1.f)
        {
            phase -= std::floor(phase);
        }
    }

    return pcm;
}

} // namespace sgl
