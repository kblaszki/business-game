#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include <gtest/gtest.h>
#include <sgl/input/InputEvent.hpp>
#include <sgl/input/Key.hpp>
#include <sgl/input/MouseButton.hpp>
#include <sgl/sfml/EventTranslate.hpp>
#include <variant>

TEST(EventTranslateTest, ToKeyMapsKnownKeys)
{
    EXPECT_EQ(sgl::sfml::toKey(sf::Keyboard::Key::A), sgl::Key::A);
    EXPECT_EQ(sgl::sfml::toKey(sf::Keyboard::Key::D), sgl::Key::D);
    EXPECT_EQ(sgl::sfml::toKey(sf::Keyboard::Key::W), sgl::Key::W);
    EXPECT_EQ(sgl::sfml::toKey(sf::Keyboard::Key::S), sgl::Key::S);
    EXPECT_EQ(sgl::sfml::toKey(sf::Keyboard::Key::Z), sgl::Key::Z);
    EXPECT_EQ(sgl::sfml::toKey(sf::Keyboard::Key::X), sgl::Key::X);
    EXPECT_EQ(sgl::sfml::toKey(sf::Keyboard::Key::C), sgl::Key::C);
    EXPECT_EQ(sgl::sfml::toKey(sf::Keyboard::Key::P), sgl::Key::P);
    EXPECT_EQ(sgl::sfml::toKey(sf::Keyboard::Key::LShift), sgl::Key::LShift);
    EXPECT_EQ(sgl::sfml::toKey(sf::Keyboard::Key::Left), sgl::Key::Left);
    EXPECT_EQ(sgl::sfml::toKey(sf::Keyboard::Key::Right), sgl::Key::Right);
    EXPECT_EQ(sgl::sfml::toKey(sf::Keyboard::Key::Up), sgl::Key::Up);
    EXPECT_EQ(sgl::sfml::toKey(sf::Keyboard::Key::Down), sgl::Key::Down);
    EXPECT_EQ(sgl::sfml::toKey(sf::Keyboard::Key::Enter), sgl::Key::Enter);
    EXPECT_EQ(sgl::sfml::toKey(sf::Keyboard::Key::Escape), sgl::Key::Escape);
    EXPECT_EQ(sgl::sfml::toKey(sf::Keyboard::Key::Backspace), sgl::Key::Backspace);
    EXPECT_EQ(sgl::sfml::toKey(sf::Keyboard::Key::Space), sgl::Key::Space);
}

TEST(EventTranslateTest, ToKeyMapsUnknown)
{
    EXPECT_EQ(sgl::sfml::toKey(sf::Keyboard::Key::B), sgl::Key::Unknown);
    EXPECT_EQ(sgl::sfml::toKey(sf::Keyboard::Key::F1), sgl::Key::Unknown);
    EXPECT_EQ(sgl::sfml::toKey(sf::Keyboard::Key::Unknown), sgl::Key::Unknown);
}

TEST(EventTranslateTest, TranslateClosed)
{
    const auto event = sgl::sfml::translate(sf::Event{sf::Event::Closed{}}, {});
    ASSERT_TRUE(event.has_value());
    EXPECT_TRUE(std::holds_alternative<sgl::WindowClosed>(*event));
}

TEST(EventTranslateTest, TranslateFocusLostAndGained)
{
    const auto lost = sgl::sfml::translate(sf::Event{sf::Event::FocusLost{}}, {});
    ASSERT_TRUE(lost.has_value());
    EXPECT_TRUE(std::holds_alternative<sgl::FocusLost>(*lost));

    const auto gained = sgl::sfml::translate(sf::Event{sf::Event::FocusGained{}}, {});
    ASSERT_TRUE(gained.has_value());
    EXPECT_TRUE(std::holds_alternative<sgl::FocusGained>(*gained));
}

TEST(EventTranslateTest, TranslateKeyPressedEnter)
{
    sf::Event::KeyPressed pressed{};
    pressed.code = sf::Keyboard::Key::Enter;
    const auto event = sgl::sfml::translate(sf::Event{pressed}, {});
    ASSERT_TRUE(event.has_value());
    const auto* down = std::get_if<sgl::KeyDown>(&*event);
    ASSERT_NE(down, nullptr);
    EXPECT_EQ(down->key, sgl::Key::Enter);
}

TEST(EventTranslateTest, TranslateKeyReleasedEscape)
{
    sf::Event::KeyReleased released{};
    released.code = sf::Keyboard::Key::Escape;
    const auto event = sgl::sfml::translate(sf::Event{released}, {});
    ASSERT_TRUE(event.has_value());
    const auto* up = std::get_if<sgl::KeyUp>(&*event);
    ASSERT_NE(up, nullptr);
    EXPECT_EQ(up->key, sgl::Key::Escape);
}

TEST(EventTranslateTest, TranslateMouseButtonPressedUsesDesignPosition)
{
    sf::Event::MouseButtonPressed pressed{};
    pressed.button = sf::Mouse::Button::Left;
    pressed.position = {10, 20};
    const sf::Vector2f design{100.f, 200.f};
    const auto event = sgl::sfml::translate(sf::Event{pressed}, design);
    ASSERT_TRUE(event.has_value());
    const auto* down = std::get_if<sgl::MouseDown>(&*event);
    ASSERT_NE(down, nullptr);
    EXPECT_EQ(down->button, sgl::MouseButton::Left);
    EXPECT_FLOAT_EQ(down->pos.x, 100.f);
    EXPECT_FLOAT_EQ(down->pos.y, 200.f);
}

TEST(EventTranslateTest, TranslateMouseButtonReleasedUsesDesignPosition)
{
    sf::Event::MouseButtonReleased released{};
    released.button = sf::Mouse::Button::Left;
    released.position = {10, 20};
    const sf::Vector2f design{50.f, 75.f};
    const auto event = sgl::sfml::translate(sf::Event{released}, design);
    ASSERT_TRUE(event.has_value());
    const auto* up = std::get_if<sgl::MouseUp>(&*event);
    ASSERT_NE(up, nullptr);
    EXPECT_EQ(up->button, sgl::MouseButton::Left);
    EXPECT_FLOAT_EQ(up->pos.x, 50.f);
    EXPECT_FLOAT_EQ(up->pos.y, 75.f);
}
