#include <gtest/gtest.h>
#include <set>
#include <string_view>
#include <tetris/app/Sounds.hpp>

TEST(SoundsTest, everySpecNonEmptyAndUniqueKey)
{
    const auto specs = sgl::tetris::soundSpecs();
    ASSERT_FALSE(specs.empty());

    std::set<std::string_view> keys;
    for(const sgl::tetris::SoundSpec& spec: specs)
    {
        EXPECT_FALSE(spec.key.empty());
        EXPECT_GT(spec.pcm.sampleRate, 0u);
        EXPECT_FALSE(spec.pcm.samples.empty());
        EXPECT_TRUE(keys.insert(spec.key).second) << spec.key;
    }
}
