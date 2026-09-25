#pragma once

namespace sgl
{

enum class SaveError
{
    NotFound,
    Io,
    Corrupt,
};

[[nodiscard]] const char* describe(SaveError error) noexcept;

} // namespace sgl
