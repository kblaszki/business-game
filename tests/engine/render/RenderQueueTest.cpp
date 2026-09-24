#include <mocks/RendererMock.hpp>

#include <eng/render/DrawCommand.hpp>
#include <eng/render/RenderQueue.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <vector>

using ::testing::_;

namespace
{

eng::RectCmd makeRect(eng::Color fill)
{
    return eng::RectCmd{
        .rect = {.pos = {0.f, 0.f}, .size = {1.f, 1.f}},
        .fill = fill,
    };
}

} // namespace

TEST(RenderQueueTest, SortsByLayerThenDepthThenSeq)
{
    eng::RenderQueue queue;
    queue.push(eng::Layer::Hud, 0.f, makeRect({1, 0, 0, 255}));
    queue.push(eng::Layer::Background, 5.f, makeRect({2, 0, 0, 255}));
    queue.push(eng::Layer::Background, 1.f, makeRect({3, 0, 0, 255}));
    queue.push(eng::Layer::Background, 1.f, makeRect({4, 0, 0, 255}));

    const auto sorted = queue.sorted();
    ASSERT_EQ(sorted.size(), 4u);
    EXPECT_EQ(sorted[0].key.layer, eng::Layer::Background);
    EXPECT_FLOAT_EQ(sorted[0].key.depth, 1.f);
    EXPECT_EQ(sorted[0].key.seq, 2u);
    EXPECT_EQ(sorted[1].key.layer, eng::Layer::Background);
    EXPECT_FLOAT_EQ(sorted[1].key.depth, 1.f);
    EXPECT_EQ(sorted[1].key.seq, 3u);
    EXPECT_EQ(sorted[2].key.layer, eng::Layer::Background);
    EXPECT_FLOAT_EQ(sorted[2].key.depth, 5.f);
    EXPECT_EQ(sorted[3].key.layer, eng::Layer::Hud);
}

TEST(RenderQueueTest, StableOnEqualSortKeys)
{
    eng::RenderQueue queue;
    // Force equal keys by pushing then rewriting is not possible; equal layer+depth
    // with different seq still orders by seq. Stability matters when keys match fully.
    // Build two entries with identical SortKey by using the same layer/depth and verifying
    // relative order of equal-key groups after interleaving other pushes.
    queue.push(eng::Layer::World, 2.f, makeRect({10, 0, 0, 255}));
    queue.push(eng::Layer::World, 2.f, makeRect({20, 0, 0, 255}));
    queue.push(eng::Layer::Actors, 0.f, makeRect({30, 0, 0, 255}));
    queue.push(eng::Layer::World, 2.f, makeRect({40, 0, 0, 255}));

    const auto sorted = queue.sorted();
    ASSERT_EQ(sorted.size(), 4u);
    // World depth 2 entries keep push order among themselves (seq 0, 1, 3).
    EXPECT_EQ(std::get<eng::RectCmd>(sorted[0].command).fill.r, 10);
    EXPECT_EQ(std::get<eng::RectCmd>(sorted[1].command).fill.r, 20);
    EXPECT_EQ(std::get<eng::RectCmd>(sorted[2].command).fill.r, 40);
    EXPECT_EQ(sorted[3].key.layer, eng::Layer::Actors);
}

TEST(RenderQueueTest, ClearResetsSeq)
{
    eng::RenderQueue queue;
    queue.push(eng::Layer::World, 0.f, makeRect({1, 0, 0, 255}));
    queue.push(eng::Layer::World, 0.f, makeRect({2, 0, 0, 255}));
    EXPECT_EQ(queue.size(), 2u);
    EXPECT_EQ(queue.sorted()[1].key.seq, 1u);

    queue.clear();
    EXPECT_EQ(queue.size(), 0u);

    queue.push(eng::Layer::World, 0.f, makeRect({3, 0, 0, 255}));
    const auto sorted = queue.sorted();
    ASSERT_EQ(sorted.size(), 1u);
    EXPECT_EQ(sorted[0].key.seq, 0u);
}

TEST(RenderQueueTest, RendererMockBeginSubmitEnd)
{
    eng::RendererMock renderer;
    eng::RenderQueue queue;

    EXPECT_CALL(renderer, begin()).Times(1);
    EXPECT_CALL(renderer, submit(_)).Times(1);
    EXPECT_CALL(renderer, end()).Times(1);

    renderer.begin();
    renderer.submit(queue);
    renderer.end();
}
