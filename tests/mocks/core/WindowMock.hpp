/* Created by kblaszki */
#pragma once

#include <core/WindowI.hpp>

#include <gmock/gmock.h>

struct WindowMock : public WindowI
{
    MOCK_METHOD(bool, isOpen, (), (const, override));
    MOCK_METHOD(void, close, (), (override));
    MOCK_METHOD(void, clear, (const sf::Color&), (override));
    MOCK_METHOD(void,
                draw,
                (const sf::Drawable&, const sf::RenderStates&),
                (override));
    MOCK_METHOD(void, display, (), (override));
    MOCK_METHOD(bool, pollEvent, (sf::Event&), (override));
};