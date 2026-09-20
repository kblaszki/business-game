/* Created by kblaszki */
#pragma once

#include <window/WindowI.hpp>

#include <gmock/gmock.h>

class WindowMock : public WindowI
{
public:
    MOCK_METHOD(bool, isOpen, (), (const, override));
    MOCK_METHOD(void, close, (), (override));
    MOCK_METHOD(void, clear, (const sf::Color&), (override));
    MOCK_METHOD(void, draw, (const sf::Drawable&, const sf::RenderStates&), (override));
    MOCK_METHOD(void, display, (), (override));
    MOCK_METHOD(std::optional<sf::Event>, pollEvent, (), (override));
    MOCK_METHOD(void, setView, (const sf::View&), (override));
    MOCK_METHOD(sf::Vector2u, getSize, (), (const, override));
};
