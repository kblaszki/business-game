/* Created by kblaszki */

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <Action.hpp>
#include <InputMapper.hpp>
#include <gtest/gtest.h>
#include <optional>

TEST(InputMapperShould, mapEnterAndNumpadEnterToConfirm)
{
    const InputMapper mapper;

    const sf::Event enter{sf::Event::KeyPressed{.code = sf::Keyboard::Key::Enter}};
    const sf::Event numpadEnter{sf::Event::KeyPressed{.scancode = sf::Keyboard::Scan::NumpadEnter}};

    EXPECT_EQ(mapper.mapEvent(enter), Action::Confirm);
    EXPECT_EQ(mapper.mapKeyPressed(sf::Keyboard::Key::Enter), Action::Confirm);
    EXPECT_EQ(mapper.mapEvent(numpadEnter), Action::Confirm);
}

TEST(InputMapperShould, mapEscapeToPause)
{
    const InputMapper mapper;
    const sf::Event escape{sf::Event::KeyPressed{.code = sf::Keyboard::Key::Escape}};

    EXPECT_EQ(mapper.mapEvent(escape), Action::Pause);
    EXPECT_EQ(mapper.mapKeyPressed(sf::Keyboard::Key::Escape), Action::Pause);
}

TEST(InputMapperShould, ignoreKeyReleasedEscape)
{
    const InputMapper mapper;
    const sf::Event released{sf::Event::KeyReleased{.code = sf::Keyboard::Key::Escape}};

    EXPECT_EQ(mapper.mapEvent(released), std::nullopt);
}

TEST(InputMapperShould, ignoreFocusLostAndClosed)
{
    const InputMapper mapper;

    EXPECT_EQ(mapper.mapEvent(sf::Event{sf::Event::FocusLost{}}), std::nullopt);
    EXPECT_EQ(mapper.mapEvent(sf::Event{sf::Event::Closed{}}), std::nullopt);
}

TEST(InputMapperShould, ignoreUnmappedKeyPressed)
{
    const InputMapper mapper;
    const sf::Event space{sf::Event::KeyPressed{.code = sf::Keyboard::Key::Space}};

    EXPECT_EQ(mapper.mapEvent(space), std::nullopt);
    EXPECT_EQ(mapper.mapKeyPressed(sf::Keyboard::Key::A), std::nullopt);
}
