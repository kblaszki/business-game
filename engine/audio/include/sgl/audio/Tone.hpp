#pragma once

#include <cstdint>
#include <sgl/audio/Envelope.hpp>
#include <sgl/audio/Pcm.hpp>
#include <sgl/audio/Waveform.hpp>
#include <sgl/core/Time.hpp>

namespace sgl
{

[[nodiscard]] Pcm
tone(Waveform waveform, float freqHz, Seconds duration, std::uint32_t sampleRate, Envelope envelope, float amplitude);

} // namespace sgl
