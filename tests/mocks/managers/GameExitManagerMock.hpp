/* Created by kblaszki */
#pragma once

#include <managers/GameExitManagerI.hpp>

#include <gmock/gmock.h>

struct GameExitManagerMock : public GameExitManagerI
{
    MOCK_METHOD(void, handleEvent, (const sf::Event&), (override));
    MOCK_METHOD(ExitUnRegisterer, registerExitHandler, (ExitHandler && handler), (override));
};