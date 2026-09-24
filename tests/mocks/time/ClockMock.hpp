#pragma once

#include <gmock/gmock.h>
#include <time/ClockI.hpp>

class ClockMock : public ClockI
{
public:
    MOCK_METHOD(sf::Time, restart, (), (override));
    MOCK_METHOD(sf::Time, getElapsedTime, (), (const, override));
};
