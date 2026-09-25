#pragma once

#include <cstdint>
#include <functional>

namespace sgl
{

template<class Tag>
struct Handle
{
    std::uint32_t id{};

    auto operator<=>(const Handle&) const = default;
};

} // namespace sgl

namespace std
{

template<class Tag>
struct hash<sgl::Handle<Tag>>
{
    size_t operator()(const sgl::Handle<Tag>& handle) const noexcept
    {
        return hash<std::uint32_t>{}(handle.id);
    }
};

} // namespace std
