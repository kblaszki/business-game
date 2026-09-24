#pragma once

#include <chrono>

namespace eng
{

using Seconds = std::chrono::duration<float>;

inline constexpr Seconds kTick{1.f / 60.f};

}  // namespace eng
