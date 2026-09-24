#include <gtest/gtest.h>
#include <time/FixedTimestep.hpp>
#include <vector>
#include <world/PowerUp.hpp>

TEST(PowerUpShould, fallDownwardFromSpawn)
{
    PowerUp capsule{{100.f, 100.f}, PowerUpKind::Wide};
    const float startY = capsule.position().y;

    capsule.fixedUpdate(FixedTimestep::tick);
    capsule.fixedUpdate(FixedTimestep::tick);
    capsule.fixedUpdate(FixedTimestep::tick);

    EXPECT_GT(capsule.position().y, startY);
    EXPECT_FLOAT_EQ(capsule.position().x, 100.f);
    EXPECT_TRUE(capsule.alive());
    EXPECT_EQ(capsule.kind(), PowerUpKind::Wide);
}

TEST(PowerUpShould, dieWhenOriginReachesBottom)
{
    PowerUp capsule{{100.f, 718.f}, PowerUpKind::ExtraLife};
    EXPECT_TRUE(capsule.alive());

    capsule.fixedUpdate(FixedTimestep::tick);

    EXPECT_GE(capsule.position().y, 720.f);
    EXPECT_FALSE(capsule.alive());
}

TEST(PowerUpShould, keepKindAndPoseAfterVectorReallocation)
{
    std::vector<PowerUp> capsules;
    capsules.emplace_back(sf::Vector2f{10.f, 20.f}, PowerUpKind::Wide);
    capsules.reserve(1);
    for(int i = 0; i < 32; ++i)
    {
        capsules.emplace_back(sf::Vector2f{0.f, 0.f}, PowerUpKind::Slow);
    }

    EXPECT_EQ(capsules.front().kind(), PowerUpKind::Wide);
    EXPECT_FLOAT_EQ(capsules.front().position().x, 10.f);
    EXPECT_FLOAT_EQ(capsules.front().position().y, 20.f);
}
