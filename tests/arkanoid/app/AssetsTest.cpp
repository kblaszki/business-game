#include <arkanoid/app/Assets.hpp>
#include <gtest/gtest.h>
#include <optional>
#include <sgl/core/Image.hpp>
#include <string_view>

namespace
{

[[nodiscard]] sgl::Image buildByKey(std::string_view key)
{
    for(const sgl::arkanoid::TextureSpec& spec: sgl::arkanoid::textureSpecs())
    {
        if(spec.key == key)
        {
            return spec.build();
        }
    }
    ADD_FAILURE() << "missing texture spec for key";
    return sgl::Image{};
}

} // namespace

TEST(AssetsShould, ballIsSixteenBySixteenWithTransparentCorner)
{
    const sgl::Image ball = buildByKey(sgl::arkanoid::AssetKeys::ball);

    EXPECT_EQ(ball.width, 16u);
    EXPECT_EQ(ball.height, 16u);

    const std::optional<sgl::Color> corner = ball.at(0, 0);
    ASSERT_TRUE(corner.has_value());
    EXPECT_EQ(corner->a, 0);
}

TEST(AssetsShould, capsuleWideIsThirtyTwoBySixteen)
{
    const sgl::Image capsule = buildByKey(sgl::arkanoid::AssetKeys::capsuleWide);

    EXPECT_EQ(capsule.width, 32u);
    EXPECT_EQ(capsule.height, 16u);
}
