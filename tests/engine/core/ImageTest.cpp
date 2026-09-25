#include <gtest/gtest.h>
#include <sgl/core/Image.hpp>

TEST(ImageTest, FilledSize)
{
    const auto image = sgl::Image::filled(3, 2, sgl::Color{1, 2, 3, 4});

    EXPECT_EQ(image.width, 3u);
    EXPECT_EQ(image.height, 2u);
    ASSERT_EQ(image.pixels.size(), 6u);
    for(const auto& pixel: image.pixels)
    {
        EXPECT_EQ(pixel, (sgl::Color{1, 2, 3, 4}));
    }
}

TEST(ImageTest, AtOutOfRangeIsNullopt)
{
    const auto image = sgl::Image::filled(2, 2, sgl::Color{});

    EXPECT_FALSE(image.at(2, 0).has_value());
    EXPECT_FALSE(image.at(0, 2).has_value());
}

TEST(ImageTest, SetOutOfRangeIsFalse)
{
    auto image = sgl::Image::filled(2, 2, sgl::Color{});

    EXPECT_FALSE(image.set(2, 0, sgl::Color{9, 9, 9}));
    EXPECT_FALSE(image.set(0, 2, sgl::Color{9, 9, 9}));
}

TEST(ImageTest, SetThenAtReturnsColor)
{
    auto image = sgl::Image::filled(2, 2, sgl::Color{});
    const sgl::Color color{10, 20, 30, 40};

    ASSERT_TRUE(image.set(1, 0, color));
    const auto got = image.at(1, 0);
    ASSERT_TRUE(got.has_value());
    EXPECT_EQ(*got, color);
}
