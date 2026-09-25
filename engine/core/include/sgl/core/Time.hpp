#pragma once

#include <chrono>

namespace sgl
{

using Seconds = std::chrono::duration<float>;

inline constexpr Seconds kTick{1.f / 60.f};

} // namespace sgl
