#include <mocks/ClockMock.hpp>
#include <mocks/PlatformMock.hpp>
#include <mocks/RendererMock.hpp>

#include <eng/core/Time.hpp>
#include <eng/input/ActionId.hpp>
#include <eng/input/ActionMap.hpp>
#include <eng/input/InputEvent.hpp>
#include <eng/input/Key.hpp>
#include <eng/loop/App.hpp>
#include <eng/render/RenderQueue.hpp>
#include <eng/scene/SceneContext.hpp>
#include <eng/scene/SceneRequest.hpp>
#include <eng/scene/SceneStack.hpp>
#include <engine/scene/fakes/SceneSpy.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>
#include <optional>
#include <vector>

using ::testing::_;
using ::testing::InSequence;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::ReturnRef;

using eng::ActionId;
using eng::ActionMap;
using eng::App;
using eng::InputEvent;
using eng::Key;
using eng::KeyDown;
using eng::kTick;
using eng::QuitApp;
using eng::RendererMock;
using eng::RenderQueue;
using eng::SceneFactory;
using eng::SceneSpy;
using eng::SceneStack;
using eng::Seconds;
using eng::WindowClosed;

namespace
{

SceneFactory idlePause()
{
    return [] { return std::make_unique<SceneSpy>(); };
}

void expectRenderPass(PlatformMock& platform, RendererMock& renderer)
{
    EXPECT_CALL(platform, renderer()).WillOnce(ReturnRef(renderer));
    EXPECT_CALL(renderer, begin());
    EXPECT_CALL(renderer, submit(_));
    EXPECT_CALL(renderer, end());
}

} // namespace

TEST(AppShould, closePlatformWhenWindowClosed)
{
    NiceMock<PlatformMock> platform;
    NiceMock<ClockMock> clock;
    NiceMock<RendererMock> renderer;
    RenderQueue queue;
    SceneStack stack{idlePause()};
    stack.push(std::make_unique<SceneSpy>());

    InSequence seq;
    EXPECT_CALL(platform, isOpen()).WillOnce(Return(true));
    EXPECT_CALL(platform, poll()).WillOnce(Return(InputEvent{WindowClosed{}})).WillOnce(Return(std::nullopt));
    EXPECT_CALL(platform, close());
    EXPECT_CALL(clock, restart()).WillOnce(Return(Seconds{0.f}));
    expectRenderPass(platform, renderer);

    App{platform, clock, ActionMap{}, stack, queue}.runFrames(1);
}

TEST(AppShould, closePlatformWhenSceneRequestsQuit)
{
    NiceMock<PlatformMock> platform;
    NiceMock<ClockMock> clock;
    NiceMock<RendererMock> renderer;
    RenderQueue queue;
    SceneStack stack{idlePause()};

    auto spy = std::make_unique<SceneSpy>();
    spy->onUpdate = [](eng::SceneContext& ctx) { ctx.request(QuitApp{}); };
    stack.push(std::move(spy));

    InSequence seq;
    EXPECT_CALL(platform, isOpen()).WillOnce(Return(true));
    EXPECT_CALL(platform, poll()).WillOnce(Return(std::nullopt));
    EXPECT_CALL(clock, restart()).WillOnce(Return(kTick));
    EXPECT_CALL(platform, close());
    expectRenderPass(platform, renderer);

    App{platform, clock, ActionMap{}, stack, queue}.runFrames(1);
    EXPECT_TRUE(stack.quitRequested());
}

TEST(AppShould, deliverActionEdgesOnlyOnFirstStepOfMultiStepFrame)
{
    NiceMock<PlatformMock> platform;
    NiceMock<ClockMock> clock;
    NiceMock<RendererMock> renderer;
    RenderQueue queue;
    SceneStack stack{idlePause()};

    const ActionId jump{1};
    ActionMap map;
    map.bind(Key::Space, jump);

    std::vector<bool> pressedPerStep;
    auto spy = std::make_unique<SceneSpy>();
    spy->onUpdate = [&](eng::SceneContext& ctx) { pressedPerStep.push_back(ctx.input().action(jump).pressed); };
    stack.push(std::move(spy));

    InSequence seq;
    EXPECT_CALL(platform, isOpen()).WillOnce(Return(true));
    EXPECT_CALL(platform, poll()).WillOnce(Return(InputEvent{KeyDown{Key::Space}})).WillOnce(Return(std::nullopt));
    EXPECT_CALL(clock, restart()).WillOnce(Return(kTick + kTick + kTick));
    expectRenderPass(platform, renderer);

    App{platform, clock, std::move(map), stack, queue}.runFrames(1);

    ASSERT_EQ(pressedPerStep.size(), 3u);
    EXPECT_TRUE(pressedPerStep[0]);
    EXPECT_FALSE(pressedPerStep[1]);
    EXPECT_FALSE(pressedPerStep[2]);
}

TEST(AppShould, storeLastAlphaFromStepResult)
{
    NiceMock<PlatformMock> platform;
    NiceMock<ClockMock> clock;
    NiceMock<RendererMock> renderer;
    RenderQueue queue;
    SceneStack stack{idlePause()};
    stack.push(std::make_unique<SceneSpy>());

    EXPECT_CALL(platform, isOpen()).WillOnce(Return(true));
    EXPECT_CALL(platform, poll()).WillOnce(Return(std::nullopt));
    EXPECT_CALL(clock, restart()).WillOnce(Return(Seconds{0.5f / 60.f}));
    expectRenderPass(platform, renderer);

    App app{platform, clock, ActionMap{}, stack, queue};
    app.runFrames(1);

    EXPECT_GE(app.lastAlpha(), 0.f);
    EXPECT_LT(app.lastAlpha(), 1.f);
    EXPECT_FLOAT_EQ(app.lastAlpha(), 0.5f);
}
