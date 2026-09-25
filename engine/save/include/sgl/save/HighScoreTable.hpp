#pragma once

#include <cstdint>
#include <optional>
#include <span>
#include <vector>

namespace sgl
{

class HighScoreTable
{
public:
    explicit HighScoreTable(size_t capacity);

    [[nodiscard]] bool qualifies(uint32_t score) const;

    // Returns 0-based rank on success; nullopt if the score does not make the table.
    [[nodiscard]] std::optional<size_t> insert(uint32_t score);

    [[nodiscard]] std::span<const uint32_t> entries() const;

private:
    size_t m_capacity;
    std::vector<uint32_t> m_scores;
};

} // namespace sgl
