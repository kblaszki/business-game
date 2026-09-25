#pragma once

#include <string_view>

namespace sgl
{

enum class ResourceError
{
    NotFound,
    DecodeFailed,
    Unsupported,
};

inline std::string_view describe(ResourceError error) noexcept
{
    switch(error)
    {
        case ResourceError::NotFound:
            return "NotFound";
        case ResourceError::DecodeFailed:
            return "DecodeFailed";
        case ResourceError::Unsupported:
            return "Unsupported";
    }
    return {};
}

} // namespace sgl
