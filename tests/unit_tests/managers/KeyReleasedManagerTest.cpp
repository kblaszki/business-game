/* Created by kblaszki */

#include <managers/KeyReleasedManager.hpp>

#include <mocks/utils/ActionMock.hpp>

#include <gtest/gtest.h>

using namespace ::testing;

class KeyReleasedManagerShould : public Test
{
protected:
    StrictMock<ActionMock> actionMock{};
    KeyReleasedManager keyReleasedManager{};
};

TEST_F(KeyReleasedManagerShould, properlyRegisterKeyReleasedHandler)
{
    EXPECT_NO_THROW(keyReleasedManager.registerHandler(sf::Keyboard::Enter,
                                                       [this](const sf::Event::KeyEvent&) { actionMock.doAction(); }));
}

TEST_F(KeyReleasedManagerShould, properlyHandleKeyReleasedEvent)
{
    EXPECT_CALL(actionMock, doAction()).Times(1);

    EXPECT_NO_THROW(keyReleasedManager.registerHandler(sf::Keyboard::Enter,
                                                       [this](const sf::Event::KeyEvent&) { actionMock.doAction(); }));

    sf::Event keyReleasedEvent{};
    keyReleasedEvent.type = sf::Event::KeyReleased;
    keyReleasedEvent.key.code = sf::Keyboard::Enter;
    keyReleasedEvent.key.scancode = sf::Keyboard::Scancode::Enter;
    keyReleasedManager.handleEvent(keyReleasedEvent);
}