#include "InputMapper.hpp"

std::optional<Action> InputMapper::mapEvent(const sf::Event& event) const
{
    if(const auto* key = event.getIf<sf::Event::KeyPressed>())
    {
        return mapKeyPressed(key->code);
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
        case sf::Keyboard::Key::Backspace:
            return Action::Cancel;
        default:
            return std::nullopt;
    }
}
