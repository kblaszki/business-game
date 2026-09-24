#pragma once

#include <cstddef>
#include <eng/input/ActionId.hpp>
#include <eng/input/Key.hpp>
#include <span>
#include <vector>

namespace eng
{

class ActionMap
{
public:
    struct AxisBind
    {
        AxisId id{};
        Key negative{};
        Key positive{};
    };

    void bind(Key key, ActionId action);
    void bindAxis(Key negative, Key positive, AxisId axis);

    [[nodiscard]] std::span<const ActionId> actionsFor(Key key) const;
    [[nodiscard]] std::span<const AxisBind> axes() const;

private:
    static constexpr std::size_t keyCount = static_cast<std::size_t>(Key::Unknown) + 1;

    std::vector<ActionId> actionsByKey_[keyCount];
    std::vector<AxisBind> axes_;
};

} // namespace eng
