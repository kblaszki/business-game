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
    MouseManagerI::MoveUnRegisterer registration;
    EXPECT_NO_THROW(registration = mouseManager.registerMoveHandler([this](const auto&) { actionMock.doAction(); }));
}

TEST_F(MouseManagerShould, properlyHandleMouseMovedEvent)
{
    EXPECT_CALL(actionMock, doAction()).Times(1);

    auto registration = mouseManager.registerMoveHandler([this](const auto&) { actionMock.doAction(); });

    mouseManager.handleEvent(sf::Event{sf::Event::MouseMoved{}});
}

TEST_F(MouseManagerShould, properlyRegisterMouseButtonEventHandler)
{
    StrictMock<ActionMock> mouseButtonPressedActionMock{};
    StrictMock<ActionMock> mouseButtonReleasedActionMock{};

    MouseManagerI::ButtonUnRegisterer registration;
    EXPECT_NO_THROW(
        registration = mouseManager.registerButtonHandler(
            sf::Mouse::Button::Left, [&](const MouseButtonStatus status, const sf::Event::MouseButtonPressed&) {
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

    auto registration = mouseManager.registerButtonHandler(
        sf::Mouse::Button::Left, [&](const MouseButtonStatus status, const sf::Event::MouseButtonPressed&) {
            if(MouseButtonStatus::Pressed == status)
                mouseButtonPressedActionMock.doAction();
            else
                mouseButtonReleasedActionMock.doAction();
        });

    sf::Event::MouseButtonPressed mouseButtonPressed{};
    mouseButtonPressed.button = sf::Mouse::Button::Left;
    mouseManager.handleEvent(sf::Event{mouseButtonPressed});
}

TEST_F(MouseManagerShould, properlyHandleMouseButtonReleasedEvent)
{
    StrictMock<ActionMock> mouseButtonPressedActionMock{};
    StrictMock<ActionMock> mouseButtonReleasedActionMock{};

    EXPECT_CALL(mouseButtonReleasedActionMock, doAction()).Times(1);

    auto registration = mouseManager.registerButtonHandler(
        sf::Mouse::Button::Left, [&](const MouseButtonStatus status, const sf::Event::MouseButtonPressed&) {
            if(MouseButtonStatus::Pressed == status)
                mouseButtonPressedActionMock.doAction();
            else
                mouseButtonReleasedActionMock.doAction();
        });

    sf::Event::MouseButtonReleased mouseButtonReleased{};
    mouseButtonReleased.button = sf::Mouse::Button::Left;
    mouseManager.handleEvent(sf::Event{mouseButtonReleased});
}

TEST_F(MouseManagerShould, properlyRegisterMouseWheelScrolledEventHandler)
{
    MouseManagerI::ScrollUnRegisterer registration;
    EXPECT_NO_THROW(registration = mouseManager.registerScrollHandler([this](const auto&) { actionMock.doAction(); }));
}

TEST_F(MouseManagerShould, properlyHandleMouseWheelScrolledEvent)
{
    EXPECT_CALL(actionMock, doAction()).Times(1);

    auto registration = mouseManager.registerScrollHandler([this](const auto&) { actionMock.doAction(); });

    mouseManager.handleEvent(sf::Event{sf::Event::MouseWheelScrolled{}});
}

TEST_F(MouseManagerShould, properlyRegisterMouseStatusEventHandler)
{
    StrictMock<ActionMock> mouseEnteredActionMock{};
    StrictMock<ActionMock> mouseLeftActionMock{};

    MouseManagerI::StatusUnRegisterer registration;
    EXPECT_NO_THROW(registration = mouseManager.registerStatusHandler([&](const MouseStatus status) {
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

    auto registration = mouseManager.registerStatusHandler([&](const MouseStatus status) {
        if(MouseStatus::Entered == status)
            mouseEnteredActionMock.doAction();
        else
            mouseLeftActionMock.doAction();
    });

    mouseManager.handleEvent(sf::Event{sf::Event::MouseEntered{}});
}

TEST_F(MouseManagerShould, properlyHandleMouseLeftEvent)
{
    StrictMock<ActionMock> mouseEnteredActionMock{};
    StrictMock<ActionMock> mouseLeftActionMock{};

    EXPECT_CALL(mouseLeftActionMock, doAction()).Times(1);

    auto registration = mouseManager.registerStatusHandler([&](const MouseStatus status) {
        if(MouseStatus::Entered == status)
            mouseEnteredActionMock.doAction();
        else
            mouseLeftActionMock.doAction();
    });

    mouseManager.handleEvent(sf::Event{sf::Event::MouseLeft{}});
}
