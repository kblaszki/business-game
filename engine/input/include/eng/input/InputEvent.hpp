#pragma once

#include <eng/core/Vec2.hpp>
#include <eng/input/Key.hpp>
#include <eng/input/MouseButton.hpp>
#include <variant>

namespace eng
{

struct KeyDown
{
    Key key{};
};

struct KeyUp
{
    Key key{};
};

struct MouseMove
{
    Vec2f pos{};
};

struct MouseDown
{
    MouseButton button{};
    Vec2f pos{};
};

struct WindowClosed
{
};

struct FocusLost
{
};

struct FocusGained
{
};

using InputEvent = std::variant<KeyDown, KeyUp, MouseMove, MouseDown, WindowClosed, FocusLost, FocusGained>;

} // namespace eng
