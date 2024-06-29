/* Created by kblaszki */
#pragma once

#include <managers/MouseManagerI.hpp>

#include <gmock/gmock.h>

struct MouseManagerMock : public MouseManagerI
{
    MOCK_METHOD(void, handleEvent, (const sf::Event&), (override));
    MOCK_METHOD(MoveUnRegisterer, registerMoveHandler, (MoveHandler && handler), (override));
};