#pragma once

#include <sgl/core/Image.hpp>
#include <span>
#include <string_view>

namespace sgl::arkanoid
{

struct AssetKeys
{
    static constexpr std::string_view background{"background"};
    static constexpr std::string_view brick{"brick"};
    static constexpr std::string_view paddle{"paddle"};
    static constexpr std::string_view ball{"ball"};
    static constexpr std::string_view capsuleWide{"capsuleWide"};
    static constexpr std::string_view capsuleMulti{"capsuleMulti"};
    static constexpr std::string_view capsuleSlow{"capsuleSlow"};
    static constexpr std::string_view capsuleExtra{"capsuleExtra"};
};

inline constexpr std::string_view uiFontRelativePath{"fonts/upheavtt.ttf"};

struct TextureSpec
{
    std::string_view key;
    sgl::Image (*build)();
};

[[nodiscard]] std::span<const TextureSpec> textureSpecs();

} // namespace sgl::arkanoid
