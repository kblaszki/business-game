#include <eng/core/Time.hpp>
#include <eng/input/InputState.hpp>
#include <eng/scene/SceneStack.hpp>
#include <engine/scene/fakes/SceneSpy.hpp>
#include <gtest/gtest.h>
#include <memory>

namespace
{

eng::SceneFactory pauseOverlayFactory()
{
    return [] {
        auto overlay = std::make_unique<eng::SceneSpy>();
        overlay->traits_ = eng::SceneTraits{.opaque = false, .blocksUpdate = true, .pausable = false};
        return overlay;
    };
}

eng::SceneSpy& pushSpy(eng::SceneStack& stack, eng::SceneTraits traits = {})
{
    auto spy = std::make_unique<eng::SceneSpy>();
    spy->traits_ = traits;
    eng::SceneSpy& ref = *spy;
    stack.push(std::move(spy));
    return ref;
}

eng::InputState makeInput(bool focusLost = false)
{
    eng::InputState input;
    if(focusLost)
    {
        eng::ActionMap map;
        input.apply(eng::FocusLost{}, map);
    }
    return input;
}

} // namespace

TEST(SceneStackShould, applyDeferredRequestsAfterWalk)
{
    eng::SceneStack stack{pauseOverlayFactory()};
    eng::SceneSpy& below = pushSpy(stack, {.opaque = true, .blocksUpdate = false, .pausable = false});
    eng::SceneSpy& top = pushSpy(stack, {.opaque = false, .blocksUpdate = true, .pausable = false});

    std::uint32_t topUpdatesWhileAlive{};
    top.onUpdate = [&](eng::SceneContext& ctx) {
        topUpdatesWhileAlive = top.updateCount;
        ctx.request(eng::PopScene{});
    };

    EXPECT_EQ(stack.size(), 2u);
    stack.update(makeInput(), eng::kTick);

    EXPECT_EQ(topUpdatesWhileAlive, 1u);
    EXPECT_EQ(below.updateCount, 0u);
    EXPECT_EQ(stack.size(), 1u);
}

TEST(SceneStackShould, replaceOnEmptyActsAsPush)
{
    eng::SceneStack stack{pauseOverlayFactory()};
    eng::SceneSpy& spy = pushSpy(stack);

    spy.onUpdate = [](eng::SceneContext& ctx) {
        ctx.request(eng::PopScene{});
        ctx.request(eng::ReplaceScene{[] { return std::make_unique<eng::SceneSpy>(); }});
    };

    stack.update(makeInput(), eng::kTick);
    EXPECT_EQ(stack.size(), 1u);
    EXPECT_FALSE(stack.empty());
}

TEST(SceneStackShould, ignorePauseWhenNotPausable)
{
    eng::SceneStack stack{pauseOverlayFactory()};
    eng::SceneSpy& spy = pushSpy(stack, {.opaque = true, .blocksUpdate = true, .pausable = false});
    spy.onUpdate = [](eng::SceneContext& ctx) { ctx.request(eng::RequestPause{}); };

    stack.update(makeInput(), eng::kTick);
    EXPECT_EQ(stack.size(), 1u);
}

TEST(SceneStackShould, ignoreDoublePauseInOneUpdate)
{
    eng::SceneStack stack{pauseOverlayFactory()};
    eng::SceneSpy& spy = pushSpy(stack, {.opaque = true, .blocksUpdate = true, .pausable = true});
    spy.onUpdate = [](eng::SceneContext& ctx) {
        ctx.request(eng::RequestPause{});
        ctx.request(eng::RequestPause{});
    };

    stack.update(makeInput(), eng::kTick);
    EXPECT_EQ(stack.size(), 2u);
}

TEST(SceneStackShould, setQuitRequestedOnQuitApp)
{
    eng::SceneStack stack{pauseOverlayFactory()};
    eng::SceneSpy& spy = pushSpy(stack);
    spy.onUpdate = [](eng::SceneContext& ctx) { ctx.request(eng::QuitApp{}); };

    EXPECT_FALSE(stack.quitRequested());
    stack.update(makeInput(), eng::kTick);
    EXPECT_TRUE(stack.quitRequested());
}

TEST(SceneStackShould, pushPauseOverlayOnFocusLost)
{
    eng::SceneStack stack{pauseOverlayFactory()};
    pushSpy(stack, {.opaque = true, .blocksUpdate = true, .pausable = true});

    stack.update(makeInput(true), eng::kTick);
    EXPECT_EQ(stack.size(), 2u);
}

TEST(SceneStackShould, reportSizeAndEmpty)
{
    eng::SceneStack stack{pauseOverlayFactory()};
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
    eng::SceneStack stack{pauseOverlayFactory()};
    eng::SceneSpy& below = pushSpy(stack, {.opaque = true, .blocksUpdate = false, .pausable = false});
    eng::SceneSpy& mid = pushSpy(stack, {.opaque = false, .blocksUpdate = true, .pausable = false});
    eng::SceneSpy& top = pushSpy(stack, {.opaque = false, .blocksUpdate = false, .pausable = false});

    stack.update(makeInput(), eng::kTick);
    EXPECT_EQ(top.updateCount, 1u);
    EXPECT_EQ(mid.updateCount, 1u);
    EXPECT_EQ(below.updateCount, 0u);
}
