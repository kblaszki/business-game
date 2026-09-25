#pragma once

#include <sgl/audio/Pcm.hpp>
#include <sgl/audio/SoundId.hpp>
#include <span>
#include <string_view>

namespace sgl::tetris
{

struct SoundSpec
{
    std::string_view key;
    sgl::Pcm pcm;
};

struct SoundIds
{
    sgl::SoundId move{};
    sgl::SoundId rotate{};
    sgl::SoundId lock{};
    sgl::SoundId line1{};
    sgl::SoundId line2{};
    sgl::SoundId line3{};
    sgl::SoundId tetris{};
    sgl::SoundId levelUp{};
    sgl::SoundId gameOver{};
};

[[nodiscard]] std::span<const SoundSpec> soundSpecs();

} // namespace sgl::tetris
