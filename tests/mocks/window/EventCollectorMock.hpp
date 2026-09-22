/* Created by kblaszki */
#pragma once

#include <window/EventCollectorI.hpp>

#include <gmock/gmock.h>

struct EventCollectorMock : public EventCollectorI
{
    MOCK_METHOD(std::optional<sf::Event>, pollEvent, (), (override));
};