/* Created by kblaszki */

#include <entities/Button.hpp>
#include <managers/MouseManager.hpp>

#include <mocks/window/ScreenRendererMock.hpp>

#include <filesystem>
#include <gtest/gtest.h>
#include <resources/ResourceManager.hpp>

using namespace ::testing;

class ButtonShould : public Test
{
protected:
    ButtonShould()
        : resources{std::filesystem::path{RESOURCES_ROOT_FOR_TESTS}}
        , font{resources.getFont("fonts/prototype.ttf")}
    {
    }

    sf::Event makeButtonPressed(sf::Vector2i position) const
    {
        sf::Event::MouseButtonPressed event{};
        event.button = sf::Mouse::Button::Left;
        event.position = position;
        return sf::Event{event};
    }

    sf::Event makeMouseMoved(sf::Vector2i position) const
    {
        sf::Event::MouseMoved event{};
        event.position = position;
        return sf::Event{event};
    }

    ResourceManager resources;
    const sf::Font& font;
    MouseManager mouseManager{};
};

TEST_F(ButtonShould, invokeOnClickWhenPressedWhileHovered)
{
    bool clicked = false;
    Button button{mouseManager, font, "Go", {100.f, 100.f}, {100.f, 50.f}, [&] { clicked = true; }};

    mouseManager.handleEvent(makeMouseMoved({150, 125}));
    mouseManager.handleEvent(makeButtonPressed({150, 125}));

    EXPECT_TRUE(clicked);
}

TEST_F(ButtonShould, notInvokeOnClickWhenPressedOutsideBounds)
{
    bool clicked = false;
    Button button{mouseManager, font, "Go", {100.f, 100.f}, {100.f, 50.f}, [&] { clicked = true; }};

    mouseManager.handleEvent(makeButtonPressed({10, 10}));

    EXPECT_FALSE(clicked);
}

TEST_F(ButtonShould, drawShapeAndText)
{
    Button button{mouseManager, font, "Go", {100.f, 100.f}, {100.f, 50.f}, [] {}};
    StrictMock<ScreenRendererMock> renderer;
    EXPECT_CALL(renderer, draw(_, _)).Times(2);
    button.draw(renderer);
}
