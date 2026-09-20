/* Created by kblaszki */
#pragma once

#include <IDrawer.hpp>
#include <gmock/gmock.h>

class DrawerMock : public IDrawer
{
public:
    MOCK_METHOD(void, draw, (const sf::Drawable&, const sf::RenderStates&), (override));
};
