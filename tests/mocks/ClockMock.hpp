#pragma once

#include <gmock/gmock.h>
#include <sgl/loop/ClockI.hpp>

class ClockMock : public sgl::ClockI
{
public:
    MOCK_METHOD(sgl::Seconds, restart, (), (override));
};
