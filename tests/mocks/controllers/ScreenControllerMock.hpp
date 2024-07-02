/* Created by kblaszki */
#pragma once

#include <controllers/ScreenControllerI.hpp>

#include <gmock/gmock.h>

struct ScreenControllerMock : public ScreenControllerI
{
    MOCK_METHOD(void, setScreen, (std::unique_ptr<ScreenI>&&), (override));
    MOCK_METHOD(void, update, (), (override));
    MOCK_METHOD(void, display, (), (override));
};
