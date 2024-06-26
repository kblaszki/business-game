/* Created by kblaszki */

#include "Player.hpp"

#include <managers/KeyPressedManager.hpp>
#include <managers/KeyReleasedManager.hpp>

Player::Player(EventManagers& eventManagers)
{

    shape.setPointCount(4);

    shape.setPoint(0, sf::Vector2f(300, 200));  // Top-left
    shape.setPoint(1, sf::Vector2f(800, 200));  // Top-right
    shape.setPoint(2, sf::Vector2f(1000, 400)); // Bottom-right
    shape.setPoint(3, sf::Vector2f(100, 400));  // Bottom-left

    shape.setFillColor(sf::Color::Green);

    eventManagers.get<sf::Event::KeyPressed>().registerHandler(sf::Keyboard::Left,
                                                               [this](const sf::Event::KeyEvent&) { moveLeft = true; });
    eventManagers.get<sf::Event::KeyPressed>().registerHandler(
        sf::Keyboard::Right, [this](const sf::Event::KeyEvent&) { moveRight = true; });
    eventManagers.get<sf::Event::KeyPressed>().registerHandler(sf::Keyboard::Down,
                                                               [this](const sf::Event::KeyEvent&) { moveDown = true; });
    eventManagers.get<sf::Event::KeyPressed>().registerHandler(sf::Keyboard::Up,
                                                               [this](const sf::Event::KeyEvent&) { moveUp = true; });
    eventManagers.get<sf::Event::KeyReleased>().registerHandler(
        sf::Keyboard::Left, [this](const sf::Event::KeyEvent&) { moveLeft = false; });
    eventManagers.get<sf::Event::KeyReleased>().registerHandler(
        sf::Keyboard::Right, [this](const sf::Event::KeyEvent&) { moveRight = false; });
    eventManagers.get<sf::Event::KeyReleased>().registerHandler(
        sf::Keyboard::Down, [this](const sf::Event::KeyEvent&) { moveDown = false; });
    eventManagers.get<sf::Event::KeyReleased>().registerHandler(sf::Keyboard::Up,
                                                                [this](const sf::Event::KeyEvent&) { moveUp = false; });
}

void Player::update()
{
    if(moveLeft)
    {
        shape.move(-10.f, 0.f);
    }
    if(moveRight)
    {
        shape.move(10.f, 0.f);
    }
    if(moveDown)
    {
        shape.move(0.f, 10.f);
    }
    if(moveUp)
    {
        shape.move(0.f, -10.f);
    }
}

void Player::draw(DrawerI& drawer) const
{
    drawer.draw(shape);
}