#include <arkanoid/app/Sounds.hpp>
#include <gtest/gtest.h>
#include <set>
#include <string_view>

TEST(SoundsTest, everySpecNonEmptyAndUniqueKey)
{
    const auto specs = sgl::arkanoid::soundSpecs();
    ASSERT_FALSE(specs.empty());

    std::set<std::string_view> keys;
    for(const sgl::arkanoid::SoundSpec& spec: specs)
    {
        EXPECT_FALSE(spec.key.empty());
        EXPECT_GT(spec.pcm.sampleRate, 0u);
        EXPECT_FALSE(spec.pcm.samples.empty());
        EXPECT_TRUE(keys.insert(spec.key).second) << spec.key;
    }
}
