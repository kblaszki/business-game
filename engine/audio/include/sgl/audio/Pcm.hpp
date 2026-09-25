#pragma once

#include <cstdint>
#include <vector>

namespace sgl
{

struct Pcm
{
    std::uint32_t sampleRate{};
    std::vector<std::int16_t> samples;
};

} // namespace sgl
