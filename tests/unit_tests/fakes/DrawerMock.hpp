/* Created by kblaszki */
#pragma once

#include <window/DrawerI.hpp>

#include <gmock/gmock.h>

class DrawerMock : public DrawerI
{
public:
    MOCK_METHOD(void, draw, (const sf::Drawable&, const sf::RenderStates&), (override));
};
