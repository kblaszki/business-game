/* Created by kblaszki */

#include "InputMapper.hpp"

std::optional<Action> InputMapper::mapEvent(const sf::Event& event) const
{
    if(const auto* const keyPressed = event.getIf<sf::Event::KeyPressed>())
    {
        if(keyPressed->scancode == sf::Keyboard::Scan::NumpadEnter)
        {
            return Action::Confirm;
        }

        return mapKeyPressed(keyPressed->code);
    }

    return std::nullopt;
}

std::optional<Action> InputMapper::mapKeyPressed(sf::Keyboard::Key key) const
{
    switch(key)
    {
        case sf::Keyboard::Key::Enter:
            return Action::Confirm;
        case sf::Keyboard::Key::Escape:
            return Action::Pause;
        default:
            return std::nullopt;
    }
}
