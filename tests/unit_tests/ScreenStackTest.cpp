#include <window/DrawerI.hpp>

#include <unit_tests/fakes/ScreenSpy.hpp>

#include <gtest/gtest.h>
#include <memory>
#include <screen/ScreenStack.hpp>
#include <time/FixedTimestep.hpp>

namespace
{
class DrawerStub : public DrawerI
{
public:
    void draw(const sf::Drawable&) override {}
};

ScreenSpy& pushSpy(ScreenStack& screens)
{
    auto spy = std::make_unique<ScreenSpy>();
    ScreenSpy& screen = *spy;
    screens.push(std::move(spy));
    return screen;
}
} // namespace

TEST(ScreenStackShould, growWhenTwoScreensArePushed)
{
    ScreenStack screens;
    pushSpy(screens);
    pushSpy(screens);
    EXPECT_EQ(screens.size(), 2u);
}

TEST(ScreenStackShould, keepScreenAliveUntilDrawAfterPopDuringUpdate)
{
    ScreenStack screens;
    bool destroyed{false};
    auto spy = std::make_unique<ScreenSpy>();
    spy->destroyed = &destroyed;
    spy->popOnUpdate = true;
    spy->stack = &screens;
    screens.push(std::move(spy));

    screens.update(FixedTimestep::tick);
    EXPECT_FALSE(destroyed);
    EXPECT_EQ(screens.size(), 1u);

    DrawerStub drawer;
    screens.draw(drawer);
    EXPECT_TRUE(destroyed);
    EXPECT_TRUE(screens.empty());
}

TEST(ScreenStackShould, leaveExactlyOneScreenAfterReplace)
{
    ScreenStack screens;
    pushSpy(screens);
    screens.replace(std::make_unique<ScreenSpy>());
    EXPECT_EQ(screens.size(), 1u);
}

TEST(ScreenStackShould, skipUpdateBelowBlockingTopAndStillDrawBelowOverlay)
{
    ScreenStack screens;
    ScreenSpy& below = pushSpy(screens);
    ScreenSpy& top = pushSpy(screens);
    top.updateBlocked = true;
    top.drawBlocked = false;

    screens.update(FixedTimestep::tick);
    EXPECT_EQ(top.updateCount, 1u);
    EXPECT_EQ(below.updateCount, 0u);

    DrawerStub drawer;
    screens.draw(drawer);
    EXPECT_EQ(top.drawCount, 1u);
    EXPECT_EQ(below.drawCount, 1u);
}

TEST(ScreenStackShould, stopActionWalkWhenTopConsumes)
{
    ScreenStack screens;
    ScreenSpy& below = pushSpy(screens);
    ScreenSpy& top = pushSpy(screens);
    top.consumeAction = true;

    EXPECT_TRUE(screens.handleAction(Action::Confirm));
    EXPECT_EQ(top.handleActionCount, 1u);
    EXPECT_EQ(below.handleActionCount, 0u);
}

TEST(ScreenStackShould, ignorePauseOverlayRequestOnSpyOrEmpty)
{
    ScreenStack empty;
    empty.requestPauseOverlay();
    EXPECT_TRUE(empty.empty());

    ScreenStack screens;
    pushSpy(screens);
    screens.requestPauseOverlay();
    EXPECT_EQ(screens.size(), 1u);
    EXPECT_NE(dynamic_cast<ScreenSpy*>(screens.top()), nullptr);
}

TEST(ScreenStackShould, latchCloseRequested)
{
    ScreenStack screens;
    EXPECT_FALSE(screens.closeRequested());
    screens.requestClose();
    EXPECT_TRUE(screens.closeRequested());
    screens.requestClose();
    EXPECT_TRUE(screens.closeRequested());
}

TEST(ScreenStackShould, noOpWhenEmpty)
{
    ScreenStack screens;
    DrawerStub drawer;
    EXPECT_TRUE(screens.empty());
    EXPECT_FALSE(screens.blocksUpdate());
    EXPECT_FALSE(screens.handleEvent(sf::Event{sf::Event::Closed{}}));
    screens.update(FixedTimestep::tick);
    screens.draw(drawer);
    screens.pop();
    EXPECT_TRUE(screens.empty());
}
