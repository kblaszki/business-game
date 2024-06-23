/* Created by kblaszki */
#pragma once

#include <managers/ClosedEventManagerI.hpp>

#include <gmock/gmock.h>

struct ClosedEventManagerMock : public EventManager<sf::Event::Closed>
{
    MOCK_METHOD(void, handleEvent, (const sf::Event&), (override));
    MOCK_METHOD(ClosedHandlerUnRegisterer, registerHandler, (ClosedHandler && handler), (override));
};