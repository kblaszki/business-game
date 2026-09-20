/* Created by kblaszki */

#include "fakes/NullDrawer.hpp"

#include <entities/Button.hpp>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>

#include <CollidableI.hpp>
#include <EntityI.hpp>
#include <HitTestI.hpp>
#include <MainMenuScreen.hpp>
#include <ScreenStack.hpp>
#include <gtest/gtest.h>
#include <memory>

TEST(ButtonShould, reportHitTestWithoutBeingCollidable)
{
    const Button button{{{10.f, 20.f}, {30.f, 40.f}}, sf::Color::Green, sf::Color::White};

    const EntityI* const entity = &button;
    EXPECT_TRUE(button.contains({15, 25}));
    EXPECT_FALSE(button.contains({9, 25}));
    EXPECT_EQ(dynamic_cast<const CollidableI*>(entity), nullptr);
    EXPECT_NE(dynamic_cast<const HitTestI*>(entity), nullptr);

    NullDrawer drawer;
    button.draw(drawer);
}

TEST(ButtonShould, toggleHoverIndependentlyOfContains)
{
    Button button{{{0.f, 0.f}, {10.f, 10.f}}, sf::Color::Red, sf::Color::Yellow};
    EXPECT_FALSE(button.hovered());
    button.setHovered(true);
    EXPECT_TRUE(button.hovered());
    button.setHovered(false);
    EXPECT_FALSE(button.hovered());
}

TEST(MainMenuScreenShould, highlightExitOnHoverAndIgnoreMissedClick)
{
    ScreenStack stack;
    auto menu = std::make_unique<MainMenuScreen>(stack);
    MainMenuScreen* const menuPtr = menu.get();
    stack.requestPush(std::move(menu));
    stack.applyCommands();

    const sf::Vector2i onExit{
        static_cast<int>(MainMenuScreen::exitButton.position.x + 10.f),
        static_cast<int>(MainMenuScreen::exitButton.position.y + 10.f),
    };
    EXPECT_TRUE(stack.handleEvent(sf::Event{sf::Event::MouseMoved{.position = onExit}}));
    EXPECT_EQ(menuPtr->hover(), MainMenuScreen::Hover::Exit);

    const sf::Vector2i miss{0, 0};
    EXPECT_TRUE(stack.handleEvent(sf::Event{sf::Event::MouseMoved{.position = miss}}));
    EXPECT_EQ(menuPtr->hover(), MainMenuScreen::Hover::None);

    EXPECT_TRUE(stack.handleEvent(
        sf::Event{sf::Event::MouseButtonPressed{.button = sf::Mouse::Button::Left, .position = miss}}));
    stack.applyCommands();

    EXPECT_FALSE(stack.closeRequested());
    EXPECT_EQ(stack.size(), 1u);
    EXPECT_NE(dynamic_cast<MainMenuScreen*>(stack.top()), nullptr);
}
