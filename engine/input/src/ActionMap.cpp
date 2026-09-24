#include <eng/input/ActionMap.hpp>

namespace eng
{
namespace
{

[[nodiscard]] std::size_t keyIndex(Key key) noexcept
{
    return static_cast<std::size_t>(key);
}

} // namespace

void ActionMap::bind(Key key, ActionId action)
{
    actionsByKey_[keyIndex(key)].push_back(action);
}

void ActionMap::bindAxis(Key negative, Key positive, AxisId axis)
{
    axes_.push_back(AxisBind{.id = axis, .negative = negative, .positive = positive});
}

std::span<const ActionId> ActionMap::actionsFor(Key key) const
{
    const auto& actions = actionsByKey_[keyIndex(key)];
    return std::span<const ActionId>{actions.data(), actions.size()};
}

std::span<const ActionMap::AxisBind> ActionMap::axes() const
{
    return std::span<const AxisBind>{axes_.data(), axes_.size()};
}

} // namespace eng
