/* Created by kblaszki */

#include <managers/MouseManager.hpp>

#include <mocks/handlers/OnClickHandlerMock.hpp>
#include <mocks/managers/MouseManagerMock.hpp>
#include <mocks/utils/ActionMock.hpp>

#include <gtest/gtest.h>

using namespace ::testing;

class OnClickHandlerShould : public Test
{
protected:
    MouseManager mouseManager{};
};

TEST_F(OnClickHandlerShould, beProperlyConstructedAndDestructed)
{
    StrictMock<MouseManagerMock> mouseManagerMock{};
    StrictMock<ActionMock> actionMock{};

    EXPECT_CALL(actionMock, doAction()).Times(1);
    EXPECT_CALL(mouseManagerMock, registerButtonHandler(sf::Mouse::Button::Left, _)).Times(1).WillOnce(Return([&] {
        actionMock.doAction();
    }));

    {
        OnClickHandlerMock onClickHandler{mouseManagerMock, sf::Mouse::Button::Left};
    }
}

TEST_F(OnClickHandlerShould, invokeOnClickWhenButtonIsPressedAndHovered)
{
    OnClickHandlerMock handler{mouseManager, sf::Mouse::Button::Left};
    EXPECT_CALL(handler, isHover(100, 200)).Times(1).WillOnce(Return(true));
    EXPECT_CALL(handler, onClick()).Times(1);

    sf::Event event;
    event.type = sf::Event::MouseButtonPressed;
    event.mouseButton.button = sf::Mouse::Button::Left;
    event.mouseButton.x = 100;
    event.mouseButton.y = 200;
    mouseManager.handleEvent(event);
}

TEST_F(OnClickHandlerShould, notInvokeOnClickWhenButtonIsPressedButNotHovered)
{
    OnClickHandlerMock handler{mouseManager, sf::Mouse::Button::Left};
    EXPECT_CALL(handler, isHover(100, 200)).Times(1).WillOnce(Return(false));
    EXPECT_CALL(handler, onClick()).Times(0);

    sf::Event event;
    event.type = sf::Event::MouseButtonPressed;
    event.mouseButton.button = sf::Mouse::Button::Left;
    event.mouseButton.x = 100;
    event.mouseButton.y = 200;
    mouseManager.handleEvent(event);
}

TEST_F(OnClickHandlerShould, invokeOnUnClickWhenButtonIsReleasedAfterClick)
{
    OnClickHandlerMock handler{mouseManager, sf::Mouse::Button::Left};
    EXPECT_CALL(handler, isHover(100, 200)).Times(2).WillOnce(Return(true)).WillOnce(Return(true));
    EXPECT_CALL(handler, onClick()).Times(1);
    EXPECT_CALL(handler, onUnClick(true)).Times(1);

    sf::Event event;
    event.type = sf::Event::MouseButtonPressed;
    event.mouseButton.button = sf::Mouse::Button::Left;
    event.mouseButton.x = 100;
    event.mouseButton.y = 200;
    mouseManager.handleEvent(event);
    event.type = sf::Event::MouseButtonReleased;
    mouseManager.handleEvent(event);
}

TEST_F(OnClickHandlerShould, notInvokeOnUnClickWhenButtonIsReleasedWithoutClick)
{
    OnClickHandlerMock handler{mouseManager, sf::Mouse::Button::Left};
    EXPECT_CALL(handler, onUnClick(true)).Times(0);

    sf::Event event;
    event.type = sf::Event::MouseButtonReleased;
    event.mouseButton.button = sf::Mouse::Button::Left;
    event.mouseButton.x = 100;
    event.mouseButton.y = 200;
    mouseManager.handleEvent(event);
}