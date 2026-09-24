#include <window/DrawerI.hpp>

#include <SFML/Window/Keyboard.hpp>

#include <gtest/gtest.h>
#include <input/InputMapper.hpp>
#include <memory>
#include <screen/GameplayScreen.hpp>
#include <screen/MainMenuScreen.hpp>
#include <screen/ScreenStack.hpp>
#include <time/FixedTimestep.hpp>

namespace
{
class DrawerStub : public DrawerI
{
public:
    void draw(const sf::Drawable&) override {}
};

sf::Event keyPressed(sf::Keyboard::Key code)
{
    sf::Event::KeyPressed pressed{};
    pressed.code = code;
    return sf::Event{pressed};
}
} // namespace

TEST(MenuGameplayShould, startOnMainMenu)
{
    ScreenStack screens;
    screens.push(std::make_unique<MainMenuScreen>(screens));

    EXPECT_EQ(screens.size(), 1u);
    EXPECT_NE(dynamic_cast<MainMenuScreen*>(screens.top()), nullptr);
}

TEST(MenuGameplayShould, replaceMenuWithGameplayOnConfirm)
{
    ScreenStack screens;
    screens.push(std::make_unique<MainMenuScreen>(screens));

    const InputMapper mapper;
    const auto action = mapper.mapEvent(keyPressed(sf::Keyboard::Key::Enter));
    ASSERT_TRUE(action.has_value());
    screens.handleAction(*action);
    DrawerStub drawer;
    screens.draw(drawer);

    EXPECT_EQ(screens.size(), 1u);
    auto* play = dynamic_cast<GameplayScreen*>(screens.top());
    ASSERT_NE(play, nullptr);

    screens.update(FixedTimestep::tick);
    EXPECT_EQ(play->tickCount(), 1u);
}

TEST(MenuGameplayShould, notCountTicksWhileMenuIsTop)
{
    ScreenStack screens;
    screens.push(std::make_unique<MainMenuScreen>(screens));
    screens.update(FixedTimestep::tick);

    EXPECT_NE(dynamic_cast<MainMenuScreen*>(screens.top()), nullptr);
}

TEST(MenuGameplayShould, ignoreNonEnterKeyOnMenu)
{
    ScreenStack screens;
    screens.push(std::make_unique<MainMenuScreen>(screens));

    screens.handleEvent(keyPressed(sf::Keyboard::Key::Space));
    DrawerStub drawer;
    screens.draw(drawer);

    EXPECT_EQ(screens.size(), 1u);
    EXPECT_NE(dynamic_cast<MainMenuScreen*>(screens.top()), nullptr);
}

sf::Event clickAt(int x, int y)
{
    sf::Event::MouseButtonPressed pressed{};
    pressed.button = sf::Mouse::Button::Left;
    pressed.position = {x, y};
    return sf::Event{pressed};
}

TEST(MenuGameplayShould, startFromStartButtonClick)
{
    ScreenStack screens;
    auto menu = std::make_unique<MainMenuScreen>(screens);
    const auto start = menu->startButtonBounds();
    screens.push(std::move(menu));

    const int x = static_cast<int>(start.position.x + start.size.x / 2.f);
    const int y = static_cast<int>(start.position.y + start.size.y / 2.f);
    screens.handleEvent(clickAt(x, y));
    DrawerStub drawer;
    screens.draw(drawer);

    EXPECT_NE(dynamic_cast<GameplayScreen*>(screens.top()), nullptr);
}

TEST(MenuGameplayShould, ignoreClickOutsideButtons)
{
    ScreenStack screens;
    screens.push(std::make_unique<MainMenuScreen>(screens));

    screens.handleEvent(clickAt(8, 8));
    DrawerStub drawer;
    screens.draw(drawer);

    EXPECT_NE(dynamic_cast<MainMenuScreen*>(screens.top()), nullptr);
}

TEST(MenuGameplayShould, requestCloseOnCancelAndStayOnMenu)
{
    ScreenStack screens;
    screens.push(std::make_unique<MainMenuScreen>(screens));

    EXPECT_FALSE(screens.closeRequested());
    EXPECT_TRUE(screens.handleAction(Action::Cancel));

    EXPECT_TRUE(screens.closeRequested());
    EXPECT_EQ(screens.size(), 1u);
    EXPECT_NE(dynamic_cast<MainMenuScreen*>(screens.top()), nullptr);
}
