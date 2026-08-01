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
    EXPECT_NO_THROW(keyboardManager.registerKeyHandler(sf::Keyboard::Key::Enter,
                                                       [this](const KeyStatus status, const sf::Event::KeyPressed&) {
                                                           if(KeyStatus::Pressed == status)
                                                               keyPressedActionMock.doAction();
                                                           else
                                                               keyReleasedActionMock.doAction();
                                                       }));
}

TEST_F(KeyboardManagerShould, properlyHandleKeyPressedEvent)
{
    EXPECT_CALL(keyPressedActionMock, doAction()).Times(1);

    EXPECT_NO_THROW(keyboardManager.registerKeyHandler(sf::Keyboard::Key::Enter,
                                                       [this](const KeyStatus status, const sf::Event::KeyPressed&) {
                                                           if(KeyStatus::Pressed == status)
                                                               keyPressedActionMock.doAction();
                                                           else
                                                               keyReleasedActionMock.doAction();
                                                       }));

    sf::Event::KeyPressed keyPressed{};
    keyPressed.code = sf::Keyboard::Key::Enter;
    keyPressed.scancode = sf::Keyboard::Scancode::Enter;
    keyboardManager.handleEvent(sf::Event{keyPressed});
}

TEST_F(KeyboardManagerShould, properlyHandleKeyReleasedEvent)
{
    EXPECT_CALL(keyReleasedActionMock, doAction()).Times(1);

    EXPECT_NO_THROW(keyboardManager.registerKeyHandler(sf::Keyboard::Key::Enter,
                                                       [this](const KeyStatus status, const sf::Event::KeyPressed&) {
                                                           if(KeyStatus::Pressed == status)
                                                               keyPressedActionMock.doAction();
                                                           else
                                                               keyReleasedActionMock.doAction();
                                                       }));

    sf::Event::KeyReleased keyReleased{};
    keyReleased.code = sf::Keyboard::Key::Enter;
    keyReleased.scancode = sf::Keyboard::Scancode::Enter;
    keyboardManager.handleEvent(sf::Event{keyReleased});
}

TEST_F(KeyboardManagerShould, properlyRegisterTextHandler)
{
    EXPECT_NO_THROW(
        keyboardManager.registerTextHandler([this](const sf::Event::TextEntered&) { textActionMock.doAction(); }));
}

TEST_F(KeyboardManagerShould, properlyHandleTextEnteredEvent)
{
    EXPECT_CALL(textActionMock, doAction()).Times(1);

    EXPECT_NO_THROW(
        keyboardManager.registerTextHandler([this](const sf::Event::TextEntered&) { textActionMock.doAction(); }));

    keyboardManager.handleEvent(sf::Event{sf::Event::TextEntered{}});
}
