/* Created by kblaszki */
#pragma once

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <Action.hpp>
#include <optional>

class InputMapper
{
public:
    [[nodiscard]] std::optional<Action> mapEvent(const sf::Event& event) const;
    [[nodiscard]] std::optional<Action> mapKeyPressed(sf::Keyboard::Key key) const;
};
