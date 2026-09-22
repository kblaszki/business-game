#pragma once

#include <window/WindowI.hpp>

#include <gmock/gmock.h>

class WindowMock : public WindowI
{
public:
    MOCK_METHOD(bool, isOpen, (), (const, override));
    MOCK_METHOD(void, close, (), (override));
    MOCK_METHOD((std::optional<sf::Event>), pollEvent, (), (override));
    MOCK_METHOD(void, clear, (), (override));
    MOCK_METHOD(void, display, (), (override));
};
