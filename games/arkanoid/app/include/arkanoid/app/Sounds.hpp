#pragma once

#include <sgl/audio/Pcm.hpp>
#include <sgl/audio/SoundId.hpp>
#include <span>
#include <string_view>

namespace sgl::arkanoid
{

struct SoundSpec
{
    std::string_view key;
    sgl::Pcm pcm;
};

struct SoundIds
{
    sgl::SoundId paddle{};
    sgl::SoundId brick{};
    sgl::SoundId wall{};
    sgl::SoundId launch{};
    sgl::SoundId powerUp{};
    sgl::SoundId ballLost{};
    sgl::SoundId lifeLost{};
    sgl::SoundId stageClear{};
    sgl::SoundId gameOver{};
};

[[nodiscard]] std::span<const SoundSpec> soundSpecs();

} // namespace sgl::arkanoid
