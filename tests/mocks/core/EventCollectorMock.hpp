/* Created by kblaszki */
#pragma once

#include <core/EventCollectorI.hpp>

#include <gmock/gmock.h>

struct EventCollectorMock : public EventCollectorI
{
    MOCK_METHOD(bool, pollEvent, (sf::Event&), (override));
};