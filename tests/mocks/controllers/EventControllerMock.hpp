/* Created by kblaszki */
#pragma once

#include <controllers/EventControllerI.hpp>

#include <gmock/gmock.h>

struct EventControllerMock : public EventControllerI
{
    MOCK_METHOD(void, handleEvents, (), (override));
    MOCK_METHOD(void,
                registerEventHandler,
                (sf::Event::EventType, EventHandler&&),
                (override));
    MOCK_METHOD(void,
                unregisterEventHandler,
                (sf::Event::EventType),
                (override));
};