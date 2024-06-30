/* Created by kblaszki */

#include <managers/MouseManager.hpp>

#include <mocks/utils/ActionMock.hpp>

#include <gtest/gtest.h>

using namespace ::testing;

class MouseManagerShould : public Test
{
protected:
    StrictMock<ActionMock> actionMock{};
    MouseManager mouseManager{};
};

TEST_F(MouseManagerShould, properlyRegisterMouseMovedEventHandler)
{
    EXPECT_NO_THROW(mouseManager.registerMoveHandler([this](const auto&) { actionMock.doAction(); }));
}

TEST_F(MouseManagerShould, properlyHandleMouseMovedEvent)
{
    EXPECT_CALL(actionMock, doAction()).Times(1);

    EXPECT_NO_THROW(mouseManager.registerMoveHandler([this](const auto&) { actionMock.doAction(); }));

    sf::Event mouseMovedEvent{};
    mouseMovedEvent.type = sf::Event::MouseMoved;
    mouseManager.handleEvent(mouseMovedEvent);
}

TEST_F(MouseManagerShould, properlyRegisterMouseButtonEventHandler)
{
    StrictMock<ActionMock> mouseButtonPressedActionMock{};
    StrictMock<ActionMock> mouseButtonReleasedActionMock{};

    EXPECT_NO_THROW(mouseManager.registerButtonHandler(
        sf::Mouse::Button::Left, [&](const MouseButtonStatus status, const sf::Event::MouseButtonEvent&) {
            if(MouseButtonStatus::Pressed == status)
                mouseButtonPressedActionMock.doAction();
            else
                mouseButtonReleasedActionMock.doAction();
        }));
}

TEST_F(MouseManagerShould, properlyHandleMouseButtonPressedEvent)
{
    StrictMock<ActionMock> mouseButtonPressedActionMock{};
    StrictMock<ActionMock> mouseButtonReleasedActionMock{};

    EXPECT_CALL(mouseButtonPressedActionMock, doAction()).Times(1);

    EXPECT_NO_THROW(mouseManager.registerButtonHandler(
        sf::Mouse::Button::Left, [&](const MouseButtonStatus status, const sf::Event::MouseButtonEvent&) {
            if(MouseButtonStatus::Pressed == status)
                mouseButtonPressedActionMock.doAction();
            else
                mouseButtonReleasedActionMock.doAction();
        }));

    sf::Event mouseButtonPressedEvent{};
    mouseButtonPressedEvent.type = sf::Event::MouseButtonPressed;
    mouseButtonPressedEvent.mouseButton.button = sf::Mouse::Button::Left;
    mouseManager.handleEvent(mouseButtonPressedEvent);
}

TEST_F(MouseManagerShould, properlyHandleMouseButtonReleasedEvent)
{
    StrictMock<ActionMock> mouseButtonPressedActionMock{};
    StrictMock<ActionMock> mouseButtonReleasedActionMock{};

    EXPECT_CALL(mouseButtonReleasedActionMock, doAction()).Times(1);

    EXPECT_NO_THROW(mouseManager.registerButtonHandler(
        sf::Mouse::Button::Left, [&](const MouseButtonStatus status, const sf::Event::MouseButtonEvent&) {
            if(MouseButtonStatus::Pressed == status)
                mouseButtonPressedActionMock.doAction();
            else
                mouseButtonReleasedActionMock.doAction();
        }));

    sf::Event mouseButtonReleasedEvent{};
    mouseButtonReleasedEvent.type = sf::Event::MouseButtonReleased;
    mouseButtonReleasedEvent.mouseButton.button = sf::Mouse::Button::Left;
    mouseManager.handleEvent(mouseButtonReleasedEvent);
}

TEST_F(MouseManagerShould, properlyRegisterMouseWheelScrolledEventHandler)
{
    EXPECT_NO_THROW(mouseManager.registerScrollHandler([this](const auto&) { actionMock.doAction(); }));
}

TEST_F(MouseManagerShould, properlyHandleMouseWheelScrolledEvent)
{
    EXPECT_CALL(actionMock, doAction()).Times(1);

    EXPECT_NO_THROW(mouseManager.registerScrollHandler([this](const auto&) { actionMock.doAction(); }));

    sf::Event mouseWheelScrolledEvent{};
    mouseWheelScrolledEvent.type = sf::Event::MouseWheelScrolled;
    mouseManager.handleEvent(mouseWheelScrolledEvent);
}

TEST_F(MouseManagerShould, properlyRegisterMouseStatusEventHandler)
{
    StrictMock<ActionMock> mouseEnteredActionMock{};
    StrictMock<ActionMock> mouseLeftActionMock{};

    EXPECT_NO_THROW(mouseManager.registerStatusHandler([&](const MouseStatus status) {
        if(MouseStatus::Entered == status)
            mouseEnteredActionMock.doAction();
        else
            mouseLeftActionMock.doAction();
    }));
}

TEST_F(MouseManagerShould, properlyHandleMouseEnteredEvent)
{
    StrictMock<ActionMock> mouseEnteredActionMock{};
    StrictMock<ActionMock> mouseLeftActionMock{};

    EXPECT_CALL(mouseEnteredActionMock, doAction()).Times(1);

    EXPECT_NO_THROW(mouseManager.registerStatusHandler([&](const MouseStatus status) {
        if(MouseStatus::Entered == status)
            mouseEnteredActionMock.doAction();
        else
            mouseLeftActionMock.doAction();
    }));

    sf::Event mouseEnteredEvent{};
    mouseEnteredEvent.type = sf::Event::MouseEntered;
    mouseManager.handleEvent(mouseEnteredEvent);
}

TEST_F(MouseManagerShould, properlyHandleMouseLeftEvent)
{
    StrictMock<ActionMock> mouseEnteredActionMock{};
    StrictMock<ActionMock> mouseLeftActionMock{};

    EXPECT_CALL(mouseLeftActionMock, doAction()).Times(1);

    EXPECT_NO_THROW(mouseManager.registerStatusHandler([&](const MouseStatus status) {
        if(MouseStatus::Entered == status)
            mouseEnteredActionMock.doAction();
        else
            mouseLeftActionMock.doAction();
    }));

    sf::Event mouseLeftEvent{};
    mouseLeftEvent.type = sf::Event::MouseLeft;
    mouseManager.handleEvent(mouseLeftEvent);
}