#pragma once

#include "WindowI.hpp"

#include <SFML/Graphics.hpp>

#include <string>

class WindowSFML : public WindowI
{
public:
    WindowSFML(sf::Vector2u size, const std::string& title);

    bool isOpen() const override;
    void close() override;
    std::optional<sf::Event> pollEvent() override;
    void clear() override;
    void display() override;

private:
    sf::RenderWindow window;
};
