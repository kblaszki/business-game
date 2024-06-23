/* Created by kblaszki */

#include <managers/KeyPressedManager.hpp>

#include <mocks/utils/ActionMock.hpp>

#include <gtest/gtest.h>

using namespace ::testing;

class KeyPressedManagerShould : public Test
{
protected:
    StrictMock<ActionMock> actionMock{};
    KeyPressedManager keyPressedManager{};
};

TEST_F(KeyPressedManagerShould, properlyRegisterKeyPressedHandler)
{
    EXPECT_NO_THROW(keyPressedManager.registerHandler(
        sf::Keyboard::Enter,
        [this](const sf::Event::KeyEvent&) { actionMock.doAction(); }));
}

TEST_F(KeyPressedManagerShould, properlyHandleKeyPressedEvent)
{
    EXPECT_CALL(actionMock, doAction()).Times(1);

    EXPECT_NO_THROW(keyPressedManager.registerHandler(
        sf::Keyboard::Enter,
        [this](const sf::Event::KeyEvent&) { actionMock.doAction(); }));

    sf::Event keyPressedEvent{};
    keyPressedEvent.type = sf::Event::KeyPressed;
    keyPressedEvent.key.code = sf::Keyboard::Enter;
    keyPressedEvent.key.scancode = sf::Keyboard::Scancode::Enter;
    keyPressedManager.handleEvent(keyPressedEvent);
}