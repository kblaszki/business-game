/* Created by kblaszki */
#pragma once

#include <managers/KeyPressedManagerI.hpp>

#include <gmock/gmock.h>

struct KeyPressedManagerMock : public KeyPressedManagerI
{
    MOCK_METHOD(void, handleEvent, (const sf::Event&), (override));
    MOCK_METHOD(KeyPressedHandlerUnRegisterer,
                registerHandler,
                (sf::Keyboard::Key, KeyPressedHandler&& handler),
                (override));
};