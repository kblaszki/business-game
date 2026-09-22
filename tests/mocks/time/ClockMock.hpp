#pragma once

#include <time/ClockI.hpp>

#include <gmock/gmock.h>

class ClockMock : public ClockI
{
public:
    MOCK_METHOD(sf::Time, restart, (), (override));
    MOCK_METHOD(sf::Time, getElapsedTime, (), (const, override));
};
