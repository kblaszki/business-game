#pragma once

#include <expected>
#include <filesystem>
#include <sgl/save/SaveError.hpp>
#include <string>
#include <string_view>

namespace sgl
{

[[nodiscard]] std::expected<std::string, SaveError> readTextFile(const std::filesystem::path& path);

[[nodiscard]] std::expected<void, SaveError> writeTextFileAtomic(const std::filesystem::path& path,
                                                                 std::string_view contents);

[[nodiscard]] std::expected<std::filesystem::path, SaveError> userDataDir(std::string_view app);

} // namespace sgl
