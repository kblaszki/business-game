#include <engine/scene/fakes/SceneSpy.hpp>
#include <gtest/gtest.h>
#include <memory>
#include <sgl/core/Time.hpp>
#include <sgl/input/InputState.hpp>
#include <sgl/scene/SceneStack.hpp>

namespace
{

sgl::SceneFactory pauseOverlayFactory()
{
    return [] {
        auto overlay = std::make_unique<sgl::SceneSpy>();
        overlay->traits_ = sgl::SceneTraits{.opaque = false, .blocksUpdate = true, .pausable = false};
        return overlay;
    };
}

sgl::SceneSpy& pushSpy(sgl::SceneStack& stack, sgl::SceneTraits traits = {})
{
    auto spy = std::make_unique<sgl::SceneSpy>();
    spy->traits_ = traits;
    sgl::SceneSpy& ref = *spy;
    stack.push(std::move(spy));
    return ref;
}

sgl::InputState makeInput(bool focusLost = false)
{
    sgl::InputState input;
    if(focusLost)
    {
        sgl::ActionMap map;
        input.apply(sgl::FocusLost{}, map);
    }
    return input;
}

} // namespace

TEST(SceneStackShould, applyDeferredRequestsAfterWalk)
{
    sgl::SceneStack stack{pauseOverlayFactory()};
    sgl::SceneSpy& below = pushSpy(stack, {.opaque = true, .blocksUpdate = false, .pausable = false});
    sgl::SceneSpy& top = pushSpy(stack, {.opaque = false, .blocksUpdate = true, .pausable = false});

    std::uint32_t topUpdatesWhileAlive{};
    top.onUpdate = [&](sgl::SceneContext& ctx) {
        topUpdatesWhileAlive = top.updateCount;
        ctx.request(sgl::PopScene{});
    };

    EXPECT_EQ(stack.size(), 2u);
    stack.update(makeInput(), sgl::kTick);

    EXPECT_EQ(topUpdatesWhileAlive, 1u);
    EXPECT_EQ(below.updateCount, 0u);
    EXPECT_EQ(stack.size(), 1u);
}

TEST(SceneStackShould, replaceOnEmptyActsAsPush)
{
    sgl::SceneStack stack{pauseOverlayFactory()};
    sgl::SceneSpy& spy = pushSpy(stack);

    spy.onUpdate = [](sgl::SceneContext& ctx) {
        ctx.request(sgl::PopScene{});
        ctx.request(sgl::ReplaceScene{[] { return std::make_unique<sgl::SceneSpy>(); }});
    };

    stack.update(makeInput(), sgl::kTick);
    EXPECT_EQ(stack.size(), 1u);
    EXPECT_FALSE(stack.empty());
}

TEST(SceneStackShould, ignorePauseWhenNotPausable)
{
    sgl::SceneStack stack{pauseOverlayFactory()};
    sgl::SceneSpy& spy = pushSpy(stack, {.opaque = true, .blocksUpdate = true, .pausable = false});
    spy.onUpdate = [](sgl::SceneContext& ctx) { ctx.request(sgl::RequestPause{}); };

    stack.update(makeInput(), sgl::kTick);
    EXPECT_EQ(stack.size(), 1u);
}

TEST(SceneStackShould, ignoreDoublePauseInOneUpdate)
{
    sgl::SceneStack stack{pauseOverlayFactory()};
    sgl::SceneSpy& spy = pushSpy(stack, {.opaque = true, .blocksUpdate = true, .pausable = true});
    spy.onUpdate = [](sgl::SceneContext& ctx) {
        ctx.request(sgl::RequestPause{});
        ctx.request(sgl::RequestPause{});
    };

    stack.update(makeInput(), sgl::kTick);
    EXPECT_EQ(stack.size(), 2u);
}

TEST(SceneStackShould, setQuitRequestedOnQuitApp)
{
    sgl::SceneStack stack{pauseOverlayFactory()};
    sgl::SceneSpy& spy = pushSpy(stack);
    spy.onUpdate = [](sgl::SceneContext& ctx) { ctx.request(sgl::QuitApp{}); };

    EXPECT_FALSE(stack.quitRequested());
    stack.update(makeInput(), sgl::kTick);
    EXPECT_TRUE(stack.quitRequested());
}

TEST(SceneStackShould, pushPauseOverlayOnFocusLost)
{
    sgl::SceneStack stack{pauseOverlayFactory()};
    pushSpy(stack, {.opaque = true, .blocksUpdate = true, .pausable = true});

    stack.update(makeInput(true), sgl::kTick);
    EXPECT_EQ(stack.size(), 2u);
}

TEST(SceneStackShould, reportSizeAndEmpty)
{
    sgl::SceneStack stack{pauseOverlayFactory()};
    EXPECT_TRUE(stack.empty());
    EXPECT_EQ(stack.size(), 0u);

    pushSpy(stack);
    EXPECT_FALSE(stack.empty());
    EXPECT_EQ(stack.size(), 1u);

    pushSpy(stack);
    EXPECT_EQ(stack.size(), 2u);
}

TEST(SceneStackShould, stopUpdateWalkAtFirstBlockingScene)
{
    sgl::SceneStack stack{pauseOverlayFactory()};
    sgl::SceneSpy& below = pushSpy(stack, {.opaque = true, .blocksUpdate = false, .pausable = false});
    sgl::SceneSpy& mid = pushSpy(stack, {.opaque = false, .blocksUpdate = true, .pausable = false});
    sgl::SceneSpy& top = pushSpy(stack, {.opaque = false, .blocksUpdate = false, .pausable = false});

    stack.update(makeInput(), sgl::kTick);
    EXPECT_EQ(top.updateCount, 1u);
    EXPECT_EQ(mid.updateCount, 1u);
    EXPECT_EQ(below.updateCount, 0u);
}
