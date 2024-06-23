/* Created by kblaszki */
#pragma once

#include <managers/EventManagerI.hpp>

#include <gmock/gmock.h>

struct EventManagerMock : public EventManagerI
{
    MOCK_METHOD(void, handleEvent, (const sf::Event&), (override));
};