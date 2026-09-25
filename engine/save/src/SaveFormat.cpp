#include <cctype>
#include <charconv>
#include <sgl/save/SaveFormat.hpp>
#include <sstream>

namespace sgl
{
namespace
{

constexpr std::string_view kHeader = "sgl-highscores 1";

[[nodiscard]] bool isAllDigits(std::string_view line)
{
    if(line.empty())
    {
        return false;
    }
    for(char c: line)
    {
        if(!std::isdigit(static_cast<unsigned char>(c)))
        {
            return false;
        }
    }
    return true;
}

} // namespace

std::expected<HighScoreTable, SaveError> parse(std::string_view text, size_t capacity)
{
    std::string_view remaining = text;
    const auto firstNl = remaining.find('\n');
    std::string_view header = firstNl == std::string_view::npos ? remaining : remaining.substr(0, firstNl);
    if(!header.empty() && header.back() == '\r')
    {
        header.remove_suffix(1);
    }
    if(header != kHeader)
    {
        return std::unexpected(SaveError::Corrupt);
    }

    HighScoreTable table(capacity);
    if(firstNl == std::string_view::npos)
    {
        return table;
    }
    remaining.remove_prefix(firstNl + 1);

    while(!remaining.empty())
    {
        const auto nl = remaining.find('\n');
        std::string_view line = nl == std::string_view::npos ? remaining : remaining.substr(0, nl);
        if(!line.empty() && line.back() == '\r')
        {
            line.remove_suffix(1);
        }

        if(nl == std::string_view::npos)
        {
            remaining = {};
        }
        else
        {
            remaining.remove_prefix(nl + 1);
        }

        // Trailing blank line after the last score is allowed.
        if(line.empty())
        {
            if(remaining.empty())
            {
                break;
            }
            return std::unexpected(SaveError::Corrupt);
        }

        if(line.front() == '-')
        {
            return std::unexpected(SaveError::Corrupt);
        }
        if(!isAllDigits(line))
        {
            return std::unexpected(SaveError::Corrupt);
        }

        uint32_t value = 0;
        const auto* begin = line.data();
        const auto* end = line.data() + line.size();
        const auto result = std::from_chars(begin, end, value);
        if(result.ec != std::errc{} || result.ptr != end)
        {
            return std::unexpected(SaveError::Corrupt);
        }
        (void)table.insert(value);
    }

    return table;
}

std::string serialize(const HighScoreTable& table)
{
    std::ostringstream out;
    out << kHeader << '\n';
    for(const uint32_t score: table.entries())
    {
        out << score << '\n';
    }
    return out.str();
}

} // namespace sgl
