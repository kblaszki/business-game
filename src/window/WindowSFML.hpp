/* Created by kblaszki */
#pragma once

#include "WindowI.hpp"

#include <SFML/Graphics.hpp>

class WindowSFML : public WindowI
{
public:
    WindowSFML();
    ~WindowSFML() override = default;

    bool isOpen() const override;
    void close() override;
    void clear(const sf::Color& color = sf::Color::Black) override;
    void draw(const sf::Drawable& drawable, const sf::RenderStates& states = sf::RenderStates::Default) override;
    void display() override;
    bool pollEvent(sf::Event& event) override;

private:
    sf::RenderWindow window;
};
