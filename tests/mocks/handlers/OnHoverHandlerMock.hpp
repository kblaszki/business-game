/* Created by kblaszki */
#pragma once

#include <handlers/OnHoverHandler.hpp>

#include <gmock/gmock.h>

struct OnHoverHandlerMock : public OnHoverHandler
{
    using OnHoverHandler::OnHoverHandler;

    MOCK_METHOD(bool, isHover, (int, int), (const, override));
    MOCK_METHOD(void, onHover, (), (override));
    MOCK_METHOD(void, onHoverOut, (), (override));
};