#pragma once

#include <cstdint>

namespace eng
{

struct Color
{
    std::uint8_t r{};
    std::uint8_t g{};
    std::uint8_t b{};
    std::uint8_t a{255};

    constexpr bool operator==(const Color&) const = default;
};

}  // namespace eng
