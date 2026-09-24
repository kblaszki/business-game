#include <algorithm>
#include <eng/input/InputState.hpp>
#include <type_traits>
#include <variant>

namespace eng
{
namespace
{

[[nodiscard]] std::size_t keyIndex(Key key) noexcept
{
    return static_cast<std::size_t>(key);
}

} // namespace

void InputState::beginFrame()
{
    for(auto& entry: actions_)
    {
        entry.second.pressed = false;
        entry.second.released = false;
    }
    closeRequested_ = false;
    focusLost_ = false;
}

void InputState::apply(const InputEvent& event, const ActionMap& map)
{
    std::visit(
        [this, &map](const auto& payload) {
            using T = std::decay_t<decltype(payload)>;
            if constexpr(std::is_same_v<T, KeyDown>)
            {
                onKeyDown(payload.key, map);
            }
            else if constexpr(std::is_same_v<T, KeyUp>)
            {
                onKeyUp(payload.key, map);
            }
            else if constexpr(std::is_same_v<T, MouseMove>)
            {
                pointer_ = payload.pos;
            }
            else if constexpr(std::is_same_v<T, MouseDown>)
            {
                pointer_ = payload.pos;
            }
            else if constexpr(std::is_same_v<T, WindowClosed>)
            {
                closeRequested_ = true;
            }
            else if constexpr(std::is_same_v<T, FocusLost>)
            {
                focusLost_ = true;
                clearHeld(map);
            }
            else if constexpr(std::is_same_v<T, FocusGained>)
            {
                // no-op
            }
        },
        event);
}

ActionState InputState::action(ActionId id) const
{
    if(const ActionState* found = findAction(id))
    {
        return *found;
    }
    return ActionState{};
}

float InputState::axis(AxisId id) const
{
    const auto it = std::find_if(axes_.begin(), axes_.end(), [id](const auto& entry) { return entry.first == id; });
    if(it == axes_.end())
    {
        return 0.f;
    }
    return it->second;
}

std::optional<Vec2f> InputState::pointer() const
{
    return pointer_;
}

bool InputState::closeRequested() const
{
    return closeRequested_;
}

bool InputState::focusLost() const
{
    return focusLost_;
}

void InputState::onKeyDown(Key key, const ActionMap& map)
{
    keysHeld_[keyIndex(key)] = true;
    for(const ActionId actionId: map.actionsFor(key))
    {
        ActionState& state = stateFor(actionId);
        state.pressed = true;
        state.held = true;
    }
    recomputeAxes(map);
}

void InputState::onKeyUp(Key key, const ActionMap& map)
{
    keysHeld_[keyIndex(key)] = false;
    for(const ActionId actionId: map.actionsFor(key))
    {
        ActionState& state = stateFor(actionId);
        state.released = true;
        state.held = false;
    }
    recomputeAxes(map);
}

void InputState::clearHeld(const ActionMap& map)
{
    for(bool& held: keysHeld_)
    {
        held = false;
    }
    for(auto& entry: actions_)
    {
        entry.second.held = false;
    }
    recomputeAxes(map);
}

void InputState::recomputeAxes(const ActionMap& map)
{
    axes_.clear();
    for(const ActionMap::AxisBind& bind: map.axes())
    {
        const bool neg = keysHeld_[keyIndex(bind.negative)];
        const bool pos = keysHeld_[keyIndex(bind.positive)];
        float value = 0.f;
        if(neg && !pos)
        {
            value = -1.f;
        }
        else if(pos && !neg)
        {
            value = 1.f;
        }
        axes_.emplace_back(bind.id, value);
    }
}

ActionState& InputState::stateFor(ActionId id)
{
    const auto it =
        std::find_if(actions_.begin(), actions_.end(), [id](const auto& entry) { return entry.first == id; });
    if(it != actions_.end())
    {
        return it->second;
    }
    actions_.emplace_back(id, ActionState{});
    return actions_.back().second;
}

const ActionState* InputState::findAction(ActionId id) const
{
    const auto it =
        std::find_if(actions_.begin(), actions_.end(), [id](const auto& entry) { return entry.first == id; });
    if(it == actions_.end())
    {
        return nullptr;
    }
    return &it->second;
}

} // namespace eng
