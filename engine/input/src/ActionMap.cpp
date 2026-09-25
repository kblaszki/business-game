#include <sgl/input/ActionMap.hpp>

namespace sgl
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

std::vector<Key> ActionMap::keysFor(ActionId action) const
{
    std::vector<Key> keys;
    for(std::size_t i = 0; i < keyCount; ++i)
    {
        for(const ActionId id: actionsByKey_[i])
        {
            if(id == action)
            {
                keys.push_back(static_cast<Key>(i));
                break;
            }
        }
    }
    return keys;
}

std::span<const ActionMap::AxisBind> ActionMap::axes() const
{
    return std::span<const AxisBind>{axes_.data(), axes_.size()};
}

} // namespace sgl
