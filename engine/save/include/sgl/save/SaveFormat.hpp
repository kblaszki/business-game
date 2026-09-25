#pragma once

#include <expected>
#include <sgl/save/HighScoreTable.hpp>
#include <sgl/save/SaveError.hpp>
#include <string>
#include <string_view>

namespace sgl
{

[[nodiscard]] std::expected<HighScoreTable, SaveError> parse(std::string_view text, size_t capacity);

[[nodiscard]] std::string serialize(const HighScoreTable& table);

} // namespace sgl
