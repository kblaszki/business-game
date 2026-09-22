/* Created by kblaszki */

#include <controllers/EventController.hpp>
#include <managers/GameExitManager.hpp>
#include <managers/KeyboardManager.hpp>
#include <managers/MouseManager.hpp>
#include <screens/MenuScreen.hpp>

#include <mocks/controllers/ScreenControllerMock.hpp>
#include <mocks/window/EventCollectorMock.hpp>
#include <mocks/window/ScreenRendererMock.hpp>
#include <mocks/window/WindowMock.hpp>

#include <filesystem>
#include <gtest/gtest.h>
#include <resources/ResourceManager.hpp>

using namespace ::testing;

class MenuScreenShould : public Test
{
protected:
    MenuScreenShould()
        : resources{std::filesystem::path{RESOURCES_ROOT_FOR_TESTS}}
        , eventController{eventCollector}
    {
        eventController.emplace<GameExitManager>(window);
        eventController.emplace<KeyboardManager>();
        eventController.emplace<MouseManager>();
    }

    sf::Event makeButtonPressed(sf::Vector2i position) const
    {
        sf::Event::MouseButtonPressed event{};
        event.button = sf::Mouse::Button::Left;
        event.position = position;
        return sf::Event{event};
    }

    ResourceManager resources;
    StrictMock<EventCollectorMock> eventCollector{};
    StrictMock<WindowMock> window{};
    EventController eventController;
    StrictMock<ScreenRendererMock> screenRenderer{};
    StrictMock<ScreenControllerMock> screenUpdater{};
};

TEST_F(MenuScreenShould, drawButtonsWithoutClearingOrPresenting)
{
    MenuScreen menu{eventController, screenRenderer, screenUpdater, resources};

    EXPECT_CALL(screenRenderer, clear(_)).Times(0);
    EXPECT_CALL(screenRenderer, display()).Times(0);
    EXPECT_CALL(screenRenderer, draw(_, _)).Times(4); // 2 buttons × (shape + text)

    menu.display();
}

TEST_F(MenuScreenShould, updateWithoutCrashing)
{
    MenuScreen menu{eventController, screenRenderer, screenUpdater, resources};
    EXPECT_NO_THROW(menu.update(1.f / 60.f));
}

TEST_F(MenuScreenShould, replaceScreenWhenStartButtonIsClicked)
{
    MenuScreen menu{eventController, screenRenderer, screenUpdater, resources};

    EXPECT_CALL(screenUpdater, replaceScreen(_)).Times(1);

    eventController.get<ManagerOf::Mouse>().handleEvent(makeButtonPressed({150, 125}));
}
