#pragma once

#include <arkanoid/sim/Tuning.hpp>
#include <eng/core/Color.hpp>
#include <eng/core/Vec2.hpp>

#include <cstdint>

namespace arkanoid
{

inline constexpr eng::Vec2u designSize{static_cast<std::uint32_t>(designWidth),
                                       static_cast<std::uint32_t>(designHeight)};

inline constexpr eng::Color titleGold{255, 230, 160};
inline constexpr eng::Color hudText{240, 230, 210};
inline constexpr eng::Color bannerFill{10, 10, 18, 210};
inline constexpr eng::Color bannerTitle{255, 230, 140};
inline constexpr eng::Color bannerHint{230, 220, 200};

inline constexpr eng::Color startButton{230, 180, 50};
inline constexpr eng::Color startButtonHover{255, 210, 80};
inline constexpr eng::Color quitButton{140, 40, 50};
inline constexpr eng::Color quitButtonHover{180, 60, 70};
inline constexpr eng::Color startLabel{20, 16, 12};
inline constexpr eng::Color quitLabel{255, 230, 230};

inline constexpr eng::Vec2f hudScorePos{32.f, 16.f};
inline constexpr eng::Vec2f hudPowerPos{560.f, 16.f};
inline constexpr eng::Vec2f hudLivesPos{1040.f, 16.f};

} // namespace arkanoid
