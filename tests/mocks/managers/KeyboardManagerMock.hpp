/* Created by kblaszki */
#pragma once

#include <managers/KeyboardManagerI.hpp>

#include <gmock/gmock.h>

struct KeyboardManagerMock : public KeyboardManagerI
{
    MOCK_METHOD(void, handleEvent, (const sf::Event&), (override));
    MOCK_METHOD(KeyUnRegisterer, registerKeyHandler, (sf::Keyboard::Key, KeyHandler&& handler), (override));
    MOCK_METHOD(TextUnRegisterer, registerTextHandler, (TextHandler && handler), (override));
};