#include <gtest/gtest.h>
#include <time/FixedTimestep.hpp>
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
