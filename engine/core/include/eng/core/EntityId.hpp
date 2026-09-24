#pragma once

#include <cstdint>

namespace eng
{

struct EntityId
{
    std::uint32_t index{};
    std::uint32_t generation{};

    bool operator==(const EntityId&) const = default;
};

} // namespace eng
