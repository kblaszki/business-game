/* Created by kblaszki */
#pragma once

#include <managers/ClosedEventManagerI.hpp>

#include <gmock/gmock.h>

struct ClosedEventManagerMock : public ClosedEventManagerI
{
    MOCK_METHOD(void, handleEvent, (const sf::Event&), (override));
    MOCK_METHOD(ClosedHandlerUnRegisterer, registerClosedHandler, (ClosedHandler && handler), (override));
};