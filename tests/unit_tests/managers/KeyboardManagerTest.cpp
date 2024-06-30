/* Created by kblaszki */

#include <managers/KeyboardManager.hpp>

#include <mocks/utils/ActionMock.hpp>

#include <gtest/gtest.h>

using namespace ::testing;

class KeyboardManagerShould : public Test
{
protected:
    StrictMock<ActionMock> keyPressedActionMock{};
    StrictMock<ActionMock> keyReleasedActionMock{};
    StrictMock<ActionMock> textActionMock{};
    KeyboardManager keyboardManager{};
};

TEST_F(KeyboardManagerShould, properlyRegisterKeyHandler)
{
    EXPECT_NO_THROW(keyboardManager.registerKeyHandler(sf::Keyboard::Enter,
                                                       [this](const KeyStatus status, const sf::Event::KeyEvent&) {
                                                           if(KeyStatus::Pressed == status)
                                                               keyPressedActionMock.doAction();
                                                           else
                                                               keyReleasedActionMock.doAction();
                                                       }));
}

TEST_F(KeyboardManagerShould, properlyHandleKeyPressedEvent)
{
    EXPECT_CALL(keyPressedActionMock, doAction()).Times(1);

    EXPECT_NO_THROW(keyboardManager.registerKeyHandler(sf::Keyboard::Enter,
                                                       [this](const KeyStatus status, const sf::Event::KeyEvent&) {
                                                           if(KeyStatus::Pressed == status)
                                                               keyPressedActionMock.doAction();
                                                           else
                                                               keyReleasedActionMock.doAction();
                                                       }));

    sf::Event keyPressedEvent{};
    keyPressedEvent.type = sf::Event::KeyPressed;
    keyPressedEvent.key.code = sf::Keyboard::Enter;
    keyPressedEvent.key.scancode = sf::Keyboard::Scancode::Enter;
    keyboardManager.handleEvent(keyPressedEvent);
}

TEST_F(KeyboardManagerShould, properlyHandleKeyReleasedEvent)
{
    EXPECT_CALL(keyReleasedActionMock, doAction()).Times(1);

    EXPECT_NO_THROW(keyboardManager.registerKeyHandler(sf::Keyboard::Enter,
                                                       [this](const KeyStatus status, const sf::Event::KeyEvent&) {
                                                           if(KeyStatus::Pressed == status)
                                                               keyPressedActionMock.doAction();
                                                           else
                                                               keyReleasedActionMock.doAction();
                                                       }));

    sf::Event keyPressedEvent{};
    keyPressedEvent.type = sf::Event::KeyReleased;
    keyPressedEvent.key.code = sf::Keyboard::Enter;
    keyPressedEvent.key.scancode = sf::Keyboard::Scancode::Enter;
    keyboardManager.handleEvent(keyPressedEvent);
}

TEST_F(KeyboardManagerShould, properlyRegisterTextHandler)
{
    EXPECT_NO_THROW(
        keyboardManager.registerTextHandler([this](const sf::Event::TextEvent&) { textActionMock.doAction(); }));
}

TEST_F(KeyboardManagerShould, properlyHandleTextEnteredEvent)
{
    EXPECT_CALL(textActionMock, doAction()).Times(1);

    EXPECT_NO_THROW(
        keyboardManager.registerTextHandler([this](const sf::Event::TextEvent&) { textActionMock.doAction(); }));

    sf::Event textEvent{};
    textEvent.type = sf::Event::TextEntered;
    keyboardManager.handleEvent(textEvent);
}