#pragma once

#include <cstdint>
#include <functional>

namespace eng
{

template<class Tag>
struct Handle
{
    std::uint32_t id{};

    auto operator<=>(const Handle&) const = default;
};

} // namespace eng

namespace std
{

template<class Tag>
struct hash<eng::Handle<Tag>>
{
    size_t operator()(const eng::Handle<Tag>& handle) const noexcept
    {
        return hash<std::uint32_t>{}(handle.id);
    }
};

} // namespace std
