#include <eng/core/Image.hpp>
#include <gtest/gtest.h>

TEST(ImageTest, FilledSize)
{
    const auto image = eng::Image::filled(3, 2, eng::Color{1, 2, 3, 4});

    EXPECT_EQ(image.width, 3u);
    EXPECT_EQ(image.height, 2u);
    ASSERT_EQ(image.pixels.size(), 6u);
    for(const auto& pixel: image.pixels)
    {
        EXPECT_EQ(pixel, (eng::Color{1, 2, 3, 4}));
    }
}

TEST(ImageTest, AtOutOfRangeIsNullopt)
{
    const auto image = eng::Image::filled(2, 2, eng::Color{});

    EXPECT_FALSE(image.at(2, 0).has_value());
    EXPECT_FALSE(image.at(0, 2).has_value());
}

TEST(ImageTest, SetOutOfRangeIsFalse)
{
    auto image = eng::Image::filled(2, 2, eng::Color{});

    EXPECT_FALSE(image.set(2, 0, eng::Color{9, 9, 9}));
    EXPECT_FALSE(image.set(0, 2, eng::Color{9, 9, 9}));
}

TEST(ImageTest, SetThenAtReturnsColor)
{
    auto image = eng::Image::filled(2, 2, eng::Color{});
    const eng::Color color{10, 20, 30, 40};

    ASSERT_TRUE(image.set(1, 0, color));
    const auto got = image.at(1, 0);
    ASSERT_TRUE(got.has_value());
    EXPECT_EQ(*got, color);
}
