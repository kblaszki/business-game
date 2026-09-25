#pragma once

#include <cstdint>
#include <sgl/core/Color.hpp>
#include <sgl/core/Vec2.hpp>
#include <string_view>
#include <tetris/sim/Types.hpp>

namespace sgl::tetris
{

inline constexpr float designWidth{1280.f};
inline constexpr float designHeight{720.f};
inline constexpr float cellSize{30.f};
inline constexpr std::uint8_t ghostAlpha{70};

inline constexpr std::string_view uiFontRelativePath{"fonts/upheavtt.ttf"};

inline constexpr sgl::Color titleGold{255, 230, 160};
inline constexpr sgl::Color hudText{240, 230, 210};
inline constexpr sgl::Color wellBorder{80, 90, 120};
inline constexpr sgl::Color wellFill{12, 14, 24};
inline constexpr sgl::Color panelFill{20, 24, 40};
inline constexpr sgl::Color startButton{230, 180, 50};
inline constexpr sgl::Color startButtonHover{255, 210, 80};
inline constexpr sgl::Color quitButton{140, 40, 50};
inline constexpr sgl::Color quitButtonHover{180, 60, 70};
inline constexpr sgl::Color startLabel{20, 16, 12};
inline constexpr sgl::Color quitLabel{255, 230, 230};
inline constexpr sgl::Color overlayDim{0, 0, 0, 150};
inline constexpr sgl::Color panelBox{24, 28, 48, 230};

inline constexpr sgl::Vec2f hudScorePos{32.f, 16.f};
inline constexpr sgl::Vec2f hudLinesPos{32.f, 48.f};
inline constexpr sgl::Vec2f hudLevelPos{32.f, 80.f};

[[nodiscard]] constexpr sgl::Color pieceColor(PieceType type)
{
    switch(type)
    {
        case PieceType::I:
            return sgl::Color{0, 240, 240};
        case PieceType::O:
            return sgl::Color{240, 240, 0};
        case PieceType::T:
            return sgl::Color{160, 0, 240};
        case PieceType::S:
            return sgl::Color{0, 240, 0};
        case PieceType::Z:
            return sgl::Color{240, 0, 0};
        case PieceType::J:
            return sgl::Color{0, 0, 240};
        case PieceType::L:
            return sgl::Color{240, 160, 0};
    }
    return sgl::Color{255, 255, 255};
}

[[nodiscard]] constexpr sgl::Color ghostColor(PieceType type)
{
    sgl::Color c = pieceColor(type);
    c.a = ghostAlpha;
    return c;
}

} // namespace sgl::tetris
