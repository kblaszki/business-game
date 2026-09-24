#pragma once

#include <cstddef>
#include <eng/core/Vec2.hpp>
#include <eng/input/ActionId.hpp>
#include <eng/input/ActionMap.hpp>
#include <eng/input/ActionState.hpp>
#include <eng/input/InputEvent.hpp>
#include <eng/input/Key.hpp>
#include <optional>
#include <vector>

namespace eng
{

class InputState
{
public:
    void beginFrame();
    void apply(const InputEvent& event, const ActionMap& map);

    [[nodiscard]] ActionState action(ActionId id) const;
    [[nodiscard]] float axis(AxisId id) const;
    [[nodiscard]] std::optional<Vec2f> pointer() const;
    [[nodiscard]] bool closeRequested() const;
    [[nodiscard]] bool focusLost() const;

private:
    void onKeyDown(Key key, const ActionMap& map);
    void onKeyUp(Key key, const ActionMap& map);
    void clearHeld(const ActionMap& map);
    void recomputeAxes(const ActionMap& map);

    [[nodiscard]] ActionState& stateFor(ActionId id);
    [[nodiscard]] const ActionState* findAction(ActionId id) const;

    static constexpr std::size_t keyCount = static_cast<std::size_t>(Key::Unknown) + 1;

    std::vector<std::pair<ActionId, ActionState>> actions_;
    std::vector<std::pair<AxisId, float>> axes_;
    bool keysHeld_[keyCount]{};
    std::optional<Vec2f> pointer_;
    bool closeRequested_{};
    bool focusLost_{};
};

} // namespace eng
