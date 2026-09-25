#include <cstddef>
#include <sgl/save/HighScoreTable.hpp>

namespace sgl
{

HighScoreTable::HighScoreTable(size_t capacity)
    : m_capacity(capacity)
{
    m_scores.reserve(capacity);
}

bool HighScoreTable::qualifies(uint32_t score) const
{
    if(m_capacity == 0)
    {
        return false;
    }
    if(m_scores.size() < m_capacity)
    {
        return true;
    }
    return score > m_scores.back();
}

std::optional<size_t> HighScoreTable::insert(uint32_t score)
{
    if(!qualifies(score))
    {
        return std::nullopt;
    }

    // First index where existing score is strictly less — keeps older equals above.
    size_t rank = 0;
    while(rank < m_scores.size() && m_scores[rank] >= score)
    {
        ++rank;
    }
    m_scores.insert(m_scores.begin() + static_cast<std::ptrdiff_t>(rank), score);
    if(m_scores.size() > m_capacity)
    {
        m_scores.pop_back();
    }
    return rank;
}

std::span<const uint32_t> HighScoreTable::entries() const
{
    return m_scores;
}

} // namespace sgl
