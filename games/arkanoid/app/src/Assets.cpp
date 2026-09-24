#include <arkanoid/app/Assets.hpp>

#include <arkanoid/sim/Tuning.hpp>
#include <eng/core/Color.hpp>

#include <array>
#include <cstdint>

namespace arkanoid
{
namespace
{

constexpr eng::Color kTransparent{0, 0, 0, 0};

void fillBox(eng::Image& image, std::uint32_t x0, std::uint32_t y0, std::uint32_t x1, std::uint32_t y1, eng::Color color)
{
    for(std::uint32_t y = y0; y < y1; ++y)
    {
        for(std::uint32_t x = x0; x < x1; ++x)
        {
            image.set(x, y, color);
        }
    }
}

eng::Image makeBackground()
{
    const auto w = static_cast<std::uint32_t>(designWidth);
    const auto h = static_cast<std::uint32_t>(designHeight);
    eng::Image image = eng::Image::filled(w, h, eng::Color{12, 16, 32});
    for(std::uint32_t y = 0; y < h; y += 24)
    {
        for(std::uint32_t x = 0; x < w; ++x)
        {
            const auto pixel = image.at(x, y);
            if(!pixel)
            {
                continue;
            }
            auto c = *pixel;
            c.r = static_cast<std::uint8_t>(c.r + 8);
            c.b = static_cast<std::uint8_t>(c.b + 12);
            image.set(x, y, c);
        }
    }
    return image;
}

eng::Image makeBrick()
{
    eng::Image image = eng::Image::filled(110, 28, eng::Color{40, 20, 30});
    fillBox(image, 2, 2, 108, 26, eng::Color{255, 255, 255});
    fillBox(image, 2, 2, 108, 6, eng::Color{255, 255, 255, 80});
    return image;
}

eng::Image makePaddle()
{
    eng::Image image = eng::Image::filled(120, 20, eng::Color{20, 20, 28});
    fillBox(image, 2, 2, 118, 18, eng::Color{240, 210, 90});
    fillBox(image, 2, 2, 118, 6, eng::Color{255, 240, 160});
    return image;
}

eng::Image makeBall()
{
    eng::Image image = eng::Image::filled(16, 16, kTransparent);
    constexpr int cx = 8;
    constexpr int cy = 8;
    for(int y = 0; y < 16; ++y)
    {
        for(int x = 0; x < 16; ++x)
        {
            const int dx = x - cx;
            const int dy = y - cy;
            if(dx * dx + dy * dy <= 49)
            {
                const bool highlight = dx + dy < -2;
                image.set(static_cast<std::uint32_t>(x), static_cast<std::uint32_t>(y),
                          highlight ? eng::Color{255, 240, 220} : eng::Color{230, 90, 70});
            }
        }
    }
    return image;
}

eng::Image makeCapsule(eng::Color tint)
{
    constexpr std::uint32_t w = 32;
    constexpr std::uint32_t h = 16;
    eng::Image image = eng::Image::filled(w, h, kTransparent);
    constexpr float cx = 15.5f;
    constexpr float cy = 7.5f;
    constexpr float rx = 14.f;
    constexpr float ry = 6.f;
    for(std::uint32_t y = 0; y < h; ++y)
    {
        for(std::uint32_t x = 0; x < w; ++x)
        {
            const float dx = (static_cast<float>(x) - cx) / rx;
            const float dy = (static_cast<float>(y) - cy) / ry;
            if(dx * dx + dy * dy <= 1.f)
            {
                const bool highlight = dy < -0.35f;
                image.set(x, y, highlight ? eng::Color{255, 255, 255, 220} : tint);
            }
        }
    }
    return image;
}

eng::Image makeCapsuleWide()
{
    return makeCapsule(eng::Color{240, 210, 90});
}

eng::Image makeCapsuleMulti()
{
    return makeCapsule(eng::Color{80, 200, 230});
}

eng::Image makeCapsuleSlow()
{
    return makeCapsule(eng::Color{90, 140, 240});
}

eng::Image makeCapsuleExtra()
{
    return makeCapsule(eng::Color{80, 200, 90});
}

const std::array<TextureSpec, 8> kSpecs{{
    {AssetKeys::background, &makeBackground},
    {AssetKeys::brick, &makeBrick},
    {AssetKeys::paddle, &makePaddle},
    {AssetKeys::ball, &makeBall},
    {AssetKeys::capsuleWide, &makeCapsuleWide},
    {AssetKeys::capsuleMulti, &makeCapsuleMulti},
    {AssetKeys::capsuleSlow, &makeCapsuleSlow},
    {AssetKeys::capsuleExtra, &makeCapsuleExtra},
}};

} // namespace

std::span<const TextureSpec> textureSpecs()
{
    return std::span<const TextureSpec>{kSpecs.data(), kSpecs.size()};
}

} // namespace arkanoid
