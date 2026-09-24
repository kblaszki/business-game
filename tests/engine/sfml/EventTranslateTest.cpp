#include <eng/input/InputEvent.hpp>
#include <eng/input/Key.hpp>
#include <eng/input/MouseButton.hpp>
#include <eng/sfml/EventTranslate.hpp>

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include <gtest/gtest.h>

#include <variant>

TEST(EventTranslateTest, ToKeyMapsKnownKeys)
{
    EXPECT_EQ(eng::sfml::toKey(sf::Keyboard::Key::A), eng::Key::A);
    EXPECT_EQ(eng::sfml::toKey(sf::Keyboard::Key::D), eng::Key::D);
    EXPECT_EQ(eng::sfml::toKey(sf::Keyboard::Key::Left), eng::Key::Left);
    EXPECT_EQ(eng::sfml::toKey(sf::Keyboard::Key::Right), eng::Key::Right);
    EXPECT_EQ(eng::sfml::toKey(sf::Keyboard::Key::Up), eng::Key::Up);
    EXPECT_EQ(eng::sfml::toKey(sf::Keyboard::Key::Down), eng::Key::Down);
    EXPECT_EQ(eng::sfml::toKey(sf::Keyboard::Key::Enter), eng::Key::Enter);
    EXPECT_EQ(eng::sfml::toKey(sf::Keyboard::Key::Escape), eng::Key::Escape);
    EXPECT_EQ(eng::sfml::toKey(sf::Keyboard::Key::Backspace), eng::Key::Backspace);
    EXPECT_EQ(eng::sfml::toKey(sf::Keyboard::Key::Space), eng::Key::Space);
}

TEST(EventTranslateTest, ToKeyMapsUnknown)
{
    EXPECT_EQ(eng::sfml::toKey(sf::Keyboard::Key::B), eng::Key::Unknown);
    EXPECT_EQ(eng::sfml::toKey(sf::Keyboard::Key::F1), eng::Key::Unknown);
    EXPECT_EQ(eng::sfml::toKey(sf::Keyboard::Key::Unknown), eng::Key::Unknown);
}

TEST(EventTranslateTest, TranslateClosed)
{
    const auto event = eng::sfml::translate(sf::Event{sf::Event::Closed{}}, {});
    ASSERT_TRUE(event.has_value());
    EXPECT_TRUE(std::holds_alternative<eng::WindowClosed>(*event));
}

TEST(EventTranslateTest, TranslateFocusLostAndGained)
{
    const auto lost = eng::sfml::translate(sf::Event{sf::Event::FocusLost{}}, {});
    ASSERT_TRUE(lost.has_value());
    EXPECT_TRUE(std::holds_alternative<eng::FocusLost>(*lost));

    const auto gained = eng::sfml::translate(sf::Event{sf::Event::FocusGained{}}, {});
    ASSERT_TRUE(gained.has_value());
    EXPECT_TRUE(std::holds_alternative<eng::FocusGained>(*gained));
}

TEST(EventTranslateTest, TranslateKeyPressedEnter)
{
    sf::Event::KeyPressed pressed{};
    pressed.code = sf::Keyboard::Key::Enter;
    const auto event = eng::sfml::translate(sf::Event{pressed}, {});
    ASSERT_TRUE(event.has_value());
    const auto* down = std::get_if<eng::KeyDown>(&*event);
    ASSERT_NE(down, nullptr);
    EXPECT_EQ(down->key, eng::Key::Enter);
}

TEST(EventTranslateTest, TranslateKeyReleasedEscape)
{
    sf::Event::KeyReleased released{};
    released.code = sf::Keyboard::Key::Escape;
    const auto event = eng::sfml::translate(sf::Event{released}, {});
    ASSERT_TRUE(event.has_value());
    const auto* up = std::get_if<eng::KeyUp>(&*event);
    ASSERT_NE(up, nullptr);
    EXPECT_EQ(up->key, eng::Key::Escape);
}

TEST(EventTranslateTest, TranslateMouseButtonPressedUsesDesignPosition)
{
    sf::Event::MouseButtonPressed pressed{};
    pressed.button = sf::Mouse::Button::Left;
    pressed.position = {10, 20};
    const sf::Vector2f design{100.f, 200.f};
    const auto event = eng::sfml::translate(sf::Event{pressed}, design);
    ASSERT_TRUE(event.has_value());
    const auto* down = std::get_if<eng::MouseDown>(&*event);
    ASSERT_NE(down, nullptr);
    EXPECT_EQ(down->button, eng::MouseButton::Left);
    EXPECT_FLOAT_EQ(down->pos.x, 100.f);
    EXPECT_FLOAT_EQ(down->pos.y, 200.f);
}
