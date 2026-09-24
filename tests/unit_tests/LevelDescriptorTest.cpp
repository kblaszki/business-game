#include <algorithm>
#include <cstdint>
#include <gtest/gtest.h>
#include <span>
#include <world/LevelDescriptor.hpp>

namespace
{
[[nodiscard]] std::size_t countOnes(std::span<const std::uint8_t> mask)
{
    return static_cast<std::size_t>(std::ranges::count(mask, std::uint8_t{1}));
}
} // namespace

TEST(LevelDescriptorShould, describeStage1Grid)
{
    const LevelDescriptor& desc = levelDescriptor(LevelId::Stage1);

    EXPECT_EQ(desc.id, LevelId::Stage1);
    EXPECT_EQ(desc.columns, 10u);
    EXPECT_EQ(desc.rows, 6u);
    EXPECT_EQ(desc.palette.size(), 6u);
    EXPECT_EQ(desc.mask.size(), 60u);
    EXPECT_EQ(countOnes(desc.mask), 60u);
}

TEST(LevelDescriptorShould, makeWorldWithBrickGridAndStuckBall)
{
    const World world = makeWorld(levelDescriptor(LevelId::Stage1));

    EXPECT_EQ(world.brickCount(), 60u);
    EXPECT_TRUE(world.brickAt(0).alive());
    EXPECT_TRUE(world.ball().stuck());
    EXPECT_EQ(world.lives(), 3u);
    EXPECT_EQ(world.score(), 0u);
    EXPECT_FALSE(world.won());
    EXPECT_FALSE(world.lost());
}

TEST(LevelDescriptorShould, makeWorldStage2FromMaskOnes)
{
    const LevelDescriptor& desc = levelDescriptor(LevelId::Stage2);
    const World world = makeWorld(desc);

    ASSERT_EQ(desc.mask.size(), desc.columns * desc.rows);
    EXPECT_LT(countOnes(desc.mask), desc.mask.size());
    EXPECT_EQ(world.brickCount(), countOnes(desc.mask));
}

TEST(LevelDescriptorShould, makeWorldStage3FrameWithHollowCenter)
{
    const LevelDescriptor& desc = levelDescriptor(LevelId::Stage3);
    const World world = makeWorld(desc);

    ASSERT_EQ(desc.mask.size(), desc.columns * desc.rows);
    EXPECT_LT(world.brickCount(), 60u);
    EXPECT_EQ(world.brickCount(), countOnes(desc.mask));

    for(std::uint32_t row = 1; row + 1 < desc.rows; ++row)
    {
        for(std::uint32_t col = 1; col + 1 < desc.columns; ++col)
        {
            EXPECT_EQ(desc.mask[row * desc.columns + col], 0u);
        }
    }
}

TEST(LevelDescriptorShould, fallBackToStage1)
{
    const LevelDescriptor& desc = levelDescriptor(static_cast<LevelId>(99));

    EXPECT_EQ(desc.id, LevelId::Stage1);
    EXPECT_EQ(makeWorld(desc).brickCount(), 60u);
}
