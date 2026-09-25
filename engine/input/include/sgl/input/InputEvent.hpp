#pragma once

#include <sgl/core/Vec2.hpp>
#include <sgl/input/Key.hpp>
#include <sgl/input/MouseButton.hpp>
#include <variant>

namespace sgl
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

struct MouseUp
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

using InputEvent = std::variant<KeyDown, KeyUp, MouseMove, MouseDown, MouseUp, WindowClosed, FocusLost, FocusGained>;

} // namespace sgl
