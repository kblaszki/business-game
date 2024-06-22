/* Created by kblaszki */
#pragma once

#include <managers/EventManagerI.hpp>

#include <gmock/gmock.h>

template<sf::Event::EventType EventType>
struct EventManagerMock : public EventManagerI
{
    static constexpr sf::Event::EventType EVENT_TYPE = EventType;
    MOCK_METHOD(void, handleEvent, (const sf::Event&), (override));
};