#include <mocks/ClockMock.hpp>
#include <mocks/PlatformMock.hpp>
#include <mocks/RendererMock.hpp>

#include <engine/scene/fakes/SceneSpy.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>
#include <optional>
#include <sgl/core/Time.hpp>
#include <sgl/input/ActionId.hpp>
#include <sgl/input/ActionMap.hpp>
#include <sgl/input/InputEvent.hpp>
#include <sgl/input/Key.hpp>
#include <sgl/loop/App.hpp>
#include <sgl/render/RenderQueue.hpp>
#include <sgl/scene/SceneContext.hpp>
#include <sgl/scene/SceneRequest.hpp>
#include <sgl/scene/SceneStack.hpp>
#include <vector>

using ::testing::_;
using ::testing::InSequence;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::ReturnRef;

using sgl::ActionId;
using sgl::ActionMap;
using sgl::App;
using sgl::InputEvent;
using sgl::Key;
using sgl::KeyDown;
using sgl::kTick;
using sgl::QuitApp;
using sgl::RendererMock;
using sgl::RenderQueue;
using sgl::SceneFactory;
using sgl::SceneSpy;
using sgl::SceneStack;
using sgl::Seconds;
using sgl::WindowClosed;

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
    spy->onUpdate = [](sgl::SceneContext& ctx) { ctx.request(QuitApp{}); };
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
    spy->onUpdate = [&](sgl::SceneContext& ctx) { pressedPerStep.push_back(ctx.input().action(jump).pressed); };
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

TEST(AppTest, deliverPressedAfterZeroStepFrame)
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
    spy->onUpdate = [&](sgl::SceneContext& ctx) { pressedPerStep.push_back(ctx.input().action(jump).pressed); };
    stack.push(std::move(spy));

    InSequence seq;
    EXPECT_CALL(platform, isOpen()).WillOnce(Return(true));
    EXPECT_CALL(platform, poll()).WillOnce(Return(InputEvent{KeyDown{Key::Space}})).WillOnce(Return(std::nullopt));
    EXPECT_CALL(clock, restart()).WillOnce(Return(Seconds{0.f}));
    expectRenderPass(platform, renderer);

    EXPECT_CALL(platform, isOpen()).WillOnce(Return(true));
    EXPECT_CALL(platform, poll()).WillOnce(Return(std::nullopt));
    EXPECT_CALL(clock, restart()).WillOnce(Return(kTick));
    expectRenderPass(platform, renderer);

    App{platform, clock, std::move(map), stack, queue}.runFrames(2);

    ASSERT_EQ(pressedPerStep.size(), 1u);
    EXPECT_TRUE(pressedPerStep[0]);
}

TEST(AppTest, doNotRepeatPressedAcrossTwoStepFrame)
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
    spy->onUpdate = [&](sgl::SceneContext& ctx) { pressedPerStep.push_back(ctx.input().action(jump).pressed); };
    stack.push(std::move(spy));

    InSequence seq;
    EXPECT_CALL(platform, isOpen()).WillOnce(Return(true));
    EXPECT_CALL(platform, poll()).WillOnce(Return(InputEvent{KeyDown{Key::Space}})).WillOnce(Return(std::nullopt));
    EXPECT_CALL(clock, restart()).WillOnce(Return(kTick + kTick));
    expectRenderPass(platform, renderer);

    EXPECT_CALL(platform, isOpen()).WillOnce(Return(true));
    EXPECT_CALL(platform, poll()).WillOnce(Return(std::nullopt));
    EXPECT_CALL(clock, restart()).WillOnce(Return(kTick));
    expectRenderPass(platform, renderer);

    App{platform, clock, std::move(map), stack, queue}.runFrames(2);

    ASSERT_EQ(pressedPerStep.size(), 3u);
    EXPECT_TRUE(pressedPerStep[0]);
    EXPECT_FALSE(pressedPerStep[1]);
    EXPECT_FALSE(pressedPerStep[2]);
}

TEST(AppTest, deliverFocusLostAfterZeroStepFrame)
{
    NiceMock<PlatformMock> platform;
    NiceMock<ClockMock> clock;
    NiceMock<RendererMock> renderer;
    RenderQueue queue;
    SceneStack stack{idlePause()};

    std::vector<bool> focusLostPerStep;
    auto spy = std::make_unique<SceneSpy>();
    spy->onUpdate = [&](sgl::SceneContext& ctx) { focusLostPerStep.push_back(ctx.input().focusLost()); };
    stack.push(std::move(spy));

    InSequence seq;
    EXPECT_CALL(platform, isOpen()).WillOnce(Return(true));
    EXPECT_CALL(platform, poll()).WillOnce(Return(InputEvent{sgl::FocusLost{}})).WillOnce(Return(std::nullopt));
    EXPECT_CALL(clock, restart()).WillOnce(Return(Seconds{0.f}));
    expectRenderPass(platform, renderer);

    EXPECT_CALL(platform, isOpen()).WillOnce(Return(true));
    EXPECT_CALL(platform, poll()).WillOnce(Return(std::nullopt));
    EXPECT_CALL(clock, restart()).WillOnce(Return(kTick));
    expectRenderPass(platform, renderer);

    App{platform, clock, ActionMap{}, stack, queue}.runFrames(2);

    ASSERT_EQ(focusLostPerStep.size(), 1u);
    EXPECT_TRUE(focusLostPerStep[0]);
}

TEST(AppTest, closeWhenStackEmpty)
{
    NiceMock<PlatformMock> platform;
    NiceMock<ClockMock> clock;
    NiceMock<RendererMock> renderer;
    RenderQueue queue;
    SceneStack stack{idlePause()};

    auto spy = std::make_unique<SceneSpy>();
    spy->onUpdate = [](sgl::SceneContext& ctx) { ctx.request(sgl::PopScene{}); };
    stack.push(std::move(spy));

    InSequence seq;
    EXPECT_CALL(platform, isOpen()).WillOnce(Return(true));
    EXPECT_CALL(platform, poll()).WillOnce(Return(std::nullopt));
    EXPECT_CALL(clock, restart()).WillOnce(Return(kTick));
    EXPECT_CALL(platform, close());
    expectRenderPass(platform, renderer);

    App{platform, clock, ActionMap{}, stack, queue}.runFrames(1);
    EXPECT_TRUE(stack.empty());
}
