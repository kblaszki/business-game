#include <gtest/gtest.h>
#include <sgl/save/SaveFormat.hpp>

TEST(SaveFormatTest, roundTrip)
{
    sgl::HighScoreTable table(5);
    (void)table.insert(300);
    (void)table.insert(200);
    (void)table.insert(100);

    const std::string text = sgl::serialize(table);
    const auto parsed = sgl::parse(text, 5);
    ASSERT_TRUE(parsed.has_value());

    const auto entries = parsed->entries();
    ASSERT_EQ(entries.size(), 3u);
    EXPECT_EQ(entries[0], 300u);
    EXPECT_EQ(entries[1], 200u);
    EXPECT_EQ(entries[2], 100u);
    EXPECT_EQ(sgl::serialize(*parsed), text);
}

TEST(SaveFormatTest, rejectsWrongHeader)
{
    const auto result = sgl::parse("not-a-header\n100\n", 5);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error(), sgl::SaveError::Corrupt);
}

TEST(SaveFormatTest, rejectsNonNumeric)
{
    const auto result = sgl::parse("sgl-highscores 1\nabc\n", 5);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error(), sgl::SaveError::Corrupt);
}

TEST(SaveFormatTest, rejectsNegative)
{
    const auto result = sgl::parse("sgl-highscores 1\n-1\n", 5);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error(), sgl::SaveError::Corrupt);
}
