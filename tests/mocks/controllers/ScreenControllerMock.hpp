/* Created by kblaszki */
#pragma once

#include <controllers/ScreenControllerI.hpp>

#include <gmock/gmock.h>

struct ScreenControllerMock : public ScreenControllerI
{
    MOCK_METHOD(void, pushScreen, (std::unique_ptr<ScreenI>&&), (override));
    MOCK_METHOD(void, popScreen, (), (override));
    MOCK_METHOD(void, replaceScreen, (std::unique_ptr<ScreenI>&&), (override));
    MOCK_METHOD(void, update, (float), (override));
    MOCK_METHOD(void, display, (), (override));
};
