/* Created by kblaszki */
#pragma once

#include <managers/MouseMovedManagerI.hpp>

#include <gmock/gmock.h>

struct MouseMovedManagerMock : public EventManager<sf::Event::MouseMoved>
{
    MOCK_METHOD(void, handleEvent, (const sf::Event&), (override));
    MOCK_METHOD(MouseMovedHandlerUnRegisterer, registerHandler, (MouseMovedHandler && handler), (override));
};