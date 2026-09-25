#pragma once

#include <arkanoid/sim/Tuning.hpp>
#include <cstdint>
#include <sgl/core/Color.hpp>
#include <sgl/core/Vec2.hpp>

namespace sgl::arkanoid
{

inline constexpr sgl::Vec2u designSize{static_cast<std::uint32_t>(designWidth),
                                       static_cast<std::uint32_t>(designHeight)};

inline constexpr sgl::Color titleGold{255, 230, 160};
inline constexpr sgl::Color hudText{240, 230, 210};
inline constexpr sgl::Color bannerFill{10, 10, 18, 210};
inline constexpr sgl::Color bannerTitle{255, 230, 140};
inline constexpr sgl::Color bannerHint{230, 220, 200};

inline constexpr sgl::Color startButton{230, 180, 50};
inline constexpr sgl::Color startButtonHover{255, 210, 80};
inline constexpr sgl::Color quitButton{140, 40, 50};
inline constexpr sgl::Color quitButtonHover{180, 60, 70};
inline constexpr sgl::Color startLabel{20, 16, 12};
inline constexpr sgl::Color quitLabel{255, 230, 230};

inline constexpr sgl::Vec2f hudScorePos{32.f, 16.f};
inline constexpr sgl::Vec2f hudPowerPos{560.f, 16.f};
inline constexpr sgl::Vec2f hudLivesPos{1040.f, 16.f};

} // namespace sgl::arkanoid
