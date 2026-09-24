#pragma once

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include <eng/input/InputEvent.hpp>
#include <eng/input/Key.hpp>
#include <eng/input/MouseButton.hpp>

#include <optional>

namespace eng::sfml
{

[[nodiscard]] constexpr eng::Key toKey(sf::Keyboard::Key key) noexcept
{
    switch(key)
    {
        case sf::Keyboard::Key::A:
            return eng::Key::A;
        case sf::Keyboard::Key::D:
            return eng::Key::D;
        case sf::Keyboard::Key::Left:
            return eng::Key::Left;
        case sf::Keyboard::Key::Right:
            return eng::Key::Right;
        case sf::Keyboard::Key::Up:
            return eng::Key::Up;
        case sf::Keyboard::Key::Down:
            return eng::Key::Down;
        case sf::Keyboard::Key::Enter:
            return eng::Key::Enter;
        case sf::Keyboard::Key::Escape:
            return eng::Key::Escape;
        case sf::Keyboard::Key::Backspace:
            return eng::Key::Backspace;
        case sf::Keyboard::Key::Space:
            return eng::Key::Space;
        default:
            return eng::Key::Unknown;
    }
}

[[nodiscard]] inline std::optional<eng::MouseButton> toMouseButton(sf::Mouse::Button button) noexcept
{
    switch(button)
    {
        case sf::Mouse::Button::Left:
            return eng::MouseButton::Left;
        case sf::Mouse::Button::Right:
            return eng::MouseButton::Right;
        case sf::Mouse::Button::Middle:
            return eng::MouseButton::Middle;
        default:
            return std::nullopt;
    }
}

[[nodiscard]] inline std::optional<eng::InputEvent> translate(const sf::Event& event, sf::Vector2f designPosition)
{
    if(event.is<sf::Event::Closed>())
    {
        return eng::WindowClosed{};
    }
    if(event.is<sf::Event::FocusLost>())
    {
        return eng::FocusLost{};
    }
    if(event.is<sf::Event::FocusGained>())
    {
        return eng::FocusGained{};
    }
    if(const auto* pressed = event.getIf<sf::Event::KeyPressed>())
    {
        return eng::KeyDown{.key = toKey(pressed->code)};
    }
    if(const auto* released = event.getIf<sf::Event::KeyReleased>())
    {
        return eng::KeyUp{.key = toKey(released->code)};
    }
    if(const auto* mouseDown = event.getIf<sf::Event::MouseButtonPressed>())
    {
        const std::optional<eng::MouseButton> button = toMouseButton(mouseDown->button);
        if(!button)
        {
            return std::nullopt;
        }
        return eng::MouseDown{.button = *button, .pos = {designPosition.x, designPosition.y}};
    }
    if(event.is<sf::Event::MouseMoved>())
    {
        return eng::MouseMove{.pos = {designPosition.x, designPosition.y}};
    }
    return std::nullopt;
}

} // namespace eng::sfml
