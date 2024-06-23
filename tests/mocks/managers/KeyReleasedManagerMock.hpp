/* Created by kblaszki */
#pragma once

#include <managers/KeyReleasedManagerI.hpp>

#include <gmock/gmock.h>

struct KeyReleasedManagerMock : public KeyReleasedManagerI
{
    MOCK_METHOD(void, handleEvent, (const sf::Event&), (override));
    MOCK_METHOD(KeyReleasedHandlerUnRegisterer,
                registerHandler,
                (sf::Keyboard::Key, KeyReleasedHandler&& handler),
                (override));
};