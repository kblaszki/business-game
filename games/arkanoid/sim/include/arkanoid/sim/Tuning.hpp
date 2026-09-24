#pragma once

#include <eng/core/Vec2.hpp>

#include <cstdint>

namespace arkanoid
{

inline constexpr float designWidth{1280.f};
inline constexpr float designHeight{720.f};

inline constexpr float paddleWidth{120.f};
inline constexpr float paddleHeight{20.f};
inline constexpr float paddleY{680.f};
inline constexpr float paddleWideWidth{200.f};
inline constexpr float paddleSpeed{480.f};

inline constexpr float ballRadius{8.f};
inline constexpr float ballDiameter{ballRadius * 2.f};
inline constexpr eng::Vec2f launchVelocity{160.f, -360.f};

inline constexpr float brickWidth{110.f};
inline constexpr float brickHeight{28.f};
inline constexpr float brickGap{8.f};
inline constexpr float brickOriginY{72.f};

inline constexpr float capsuleWidth{32.f};
inline constexpr float capsuleHeight{16.f};
inline constexpr float capsuleFallSpeed{180.f};

inline constexpr float effectDuration{8.f};
inline constexpr float slowFactor{0.6f};
inline constexpr float multiballSpread{120.f};

inline constexpr std::uint32_t scorePerBrick{10};
inline constexpr std::uint32_t startingLives{3};
inline constexpr std::uint32_t levelColumns{10};
inline constexpr std::uint32_t levelRows{6};

} // namespace arkanoid
