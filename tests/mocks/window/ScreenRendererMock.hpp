/* Created by kblaszki */
#pragma once

#include <window/ScreenRendererI.hpp>

#include <gmock/gmock.h>

struct ScreenRendererMock : public ScreenRendererI
{
    MOCK_METHOD(void, clear, (const sf::Color&), (override));
    MOCK_METHOD(void, draw, (const sf::Drawable&, const sf::RenderStates&), (override));
    MOCK_METHOD(void, display, (), (override));
};
