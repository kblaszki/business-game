#pragma once

#include <version>

namespace sgl
{

static_assert(__cpp_lib_expected >= 202202L, "eng requires std::expected");
static_assert(__cpp_lib_move_only_function >= 202110L, "eng requires std::move_only_function");
static_assert(__cpp_lib_ranges >= 201911L, "eng requires std::ranges");
static_assert(__cpp_concepts >= 201907L, "eng requires concepts");

} // namespace sgl
