#include <sgl/save/SaveError.hpp>

namespace sgl
{

const char* describe(SaveError error) noexcept
{
    switch(error)
    {
        case SaveError::NotFound:
            return "NotFound";
        case SaveError::Io:
            return "Io";
        case SaveError::Corrupt:
            return "Corrupt";
    }
    return "";
}

} // namespace sgl
