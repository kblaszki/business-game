#pragma once

#include <eng/loop/ClockI.hpp>
#include <gmock/gmock.h>

class ClockMock : public eng::ClockI
{
public:
    MOCK_METHOD(eng::Seconds, restart, (), (override));
};
