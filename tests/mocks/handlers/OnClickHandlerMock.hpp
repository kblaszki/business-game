/* Created by kblaszki */
#pragma once

#include <handlers/OnClickHandler.hpp>

#include <gmock/gmock.h>

struct OnClickHandlerMock : public OnClickHandler
{
    using OnClickHandler::OnClickHandler;

    MOCK_METHOD(bool, isHover, (int, int), (const, override));
    MOCK_METHOD(void, onClick, (), (override));
    MOCK_METHOD(void, onUnClick, (bool), (override));
};