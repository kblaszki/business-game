#include <SFML/Window/Keyboard.hpp>

#include <gtest/gtest.h>
#include <input/InputMapper.hpp>

namespace
{
sf::Event keyPressed(sf::Keyboard::Key code)
{
    sf::Event::KeyPressed pressed{};
    pressed.code = code;
    return sf::Event{pressed};
}

sf::Event keyReleased(sf::Keyboard::Key code)
{
    sf::Event::KeyReleased released{};
    released.code = code;
    return sf::Event{released};
}
} // namespace

TEST(InputMapperShould, mapEnterToConfirm)
{
    const InputMapper mapper;
    EXPECT_EQ(mapper.mapKeyPressed(sf::Keyboard::Key::Enter), Action::Confirm);
}

TEST(InputMapperShould, mapEscapeToPause)
{
    const InputMapper mapper;
    EXPECT_EQ(mapper.mapKeyPressed(sf::Keyboard::Key::Escape), Action::Pause);
}

TEST(InputMapperShould, mapBackspaceToCancel)
{
    const InputMapper mapper;
    EXPECT_EQ(mapper.mapKeyPressed(sf::Keyboard::Key::Backspace), Action::Cancel);
}

TEST(InputMapperShould, ignoreUnmappedKeyReleasedAndClosed)
{
    const InputMapper mapper;
    EXPECT_EQ(mapper.mapKeyPressed(sf::Keyboard::Key::Space), std::nullopt);
    EXPECT_EQ(mapper.mapEvent(keyReleased(sf::Keyboard::Key::Enter)), std::nullopt);
    EXPECT_EQ(mapper.mapEvent(sf::Event{sf::Event::Closed{}}), std::nullopt);
    EXPECT_EQ(mapper.mapEvent(sf::Event{sf::Event::FocusLost{}}), std::nullopt);
}

TEST(InputMapperShould, mapKeyPressedEventToConfirm)
{
    const InputMapper mapper;
    EXPECT_EQ(mapper.mapEvent(keyPressed(sf::Keyboard::Key::Enter)), Action::Confirm);
}
