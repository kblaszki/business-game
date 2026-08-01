#include "Paddle.hpp"

Paddle::Paddle(KeyboardManagerI& keyboardManager)
{
    shape.setPosition({400, 500});
    shape.setSize(sf::Vector2f(100, 20));
    shape.setFillColor(sf::Color::Green);

    keyboardManager.registerKeyHandler(sf::Keyboard::Key::Left,
                                       [this](const KeyStatus status, const sf::Event::KeyPressed&) {
                                           moveLeft = KeyStatus::Pressed == status;
                                       });
    keyboardManager.registerKeyHandler(sf::Keyboard::Key::Right,
                                       [this](const KeyStatus status, const sf::Event::KeyPressed&) {
                                           moveRight = KeyStatus::Pressed == status;
                                       });
}

void Paddle::update()
{
    if(moveLeft)
    {
        shape.move({-10.f, 0.f});
    }
    if(moveRight)
    {
        shape.move({10.f, 0.f});
    }
}

void Paddle::draw(DrawerI& drawer) const
{
    drawer.draw(shape);
}