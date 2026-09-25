#include <array>
#include <gtest/gtest.h>
#include <set>
#include <tetris/sim/Bag.hpp>
#include <tetris/sim/Types.hpp>

using sgl::tetris::Bag;
using sgl::tetris::PieceType;

TEST(BagTest, eachBagOfSevenHasAllTypes)
{
    Bag bag{12345};
    for(int bagIndex = 0; bagIndex < 100; ++bagIndex)
    {
        std::set<PieceType> seen;
        for(int i = 0; i < 7; ++i)
        {
            seen.insert(bag.next());
        }
        EXPECT_EQ(seen.size(), 7u) << "bag " << bagIndex;
        EXPECT_TRUE(seen.contains(PieceType::I));
        EXPECT_TRUE(seen.contains(PieceType::O));
        EXPECT_TRUE(seen.contains(PieceType::T));
        EXPECT_TRUE(seen.contains(PieceType::S));
        EXPECT_TRUE(seen.contains(PieceType::Z));
        EXPECT_TRUE(seen.contains(PieceType::J));
        EXPECT_TRUE(seen.contains(PieceType::L));
    }
}

TEST(BagTest, sameSeedSameSequence)
{
    Bag a{99};
    Bag b{99};
    for(int i = 0; i < 50; ++i)
    {
        EXPECT_EQ(a.next(), b.next());
    }
}

TEST(BagTest, previewMatchesNext)
{
    Bag bag{7};
    for(int i = 0; i < 30; ++i)
    {
        const std::array<PieceType, 5> preview = bag.preview();
        EXPECT_EQ(bag.next(), preview[0]);
        const std::array<PieceType, 5> after = bag.preview();
        for(std::size_t j = 0; j < 4; ++j)
        {
            EXPECT_EQ(after[j], preview[j + 1]);
        }
    }
}
