#include <arkanoid/app/Assets.hpp>

#include <eng/core/Image.hpp>

#include <gtest/gtest.h>

#include <optional>
#include <string_view>

namespace
{

[[nodiscard]] eng::Image buildByKey(std::string_view key)
{
    for(const arkanoid::TextureSpec& spec: arkanoid::textureSpecs())
    {
        if(spec.key == key)
        {
            return spec.build();
        }
    }
    ADD_FAILURE() << "missing texture spec for key";
    return eng::Image{};
}

} // namespace

TEST(AssetsShould, ballIsSixteenBySixteenWithTransparentCorner)
{
    const eng::Image ball = buildByKey(arkanoid::AssetKeys::ball);

    EXPECT_EQ(ball.width, 16u);
    EXPECT_EQ(ball.height, 16u);

    const std::optional<eng::Color> corner = ball.at(0, 0);
    ASSERT_TRUE(corner.has_value());
    EXPECT_EQ(corner->a, 0);
}

TEST(AssetsShould, capsuleWideIsThirtyTwoBySixteen)
{
    const eng::Image capsule = buildByKey(arkanoid::AssetKeys::capsuleWide);

    EXPECT_EQ(capsule.width, 32u);
    EXPECT_EQ(capsule.height, 16u);
}
