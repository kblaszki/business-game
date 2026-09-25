#include <mocks/AudioSpy.hpp>

#include <gtest/gtest.h>
#include <sgl/core/Random.hpp>
#include <sgl/core/Time.hpp>
#include <span>
#include <tetris/app/Feedback.hpp>
#include <tetris/sim/Events.hpp>

namespace
{

sgl::tetris::SoundIds makeIds()
{
    return sgl::tetris::SoundIds{
        .move = sgl::SoundId{.id = 1},
        .rotate = sgl::SoundId{.id = 2},
        .lock = sgl::SoundId{.id = 3},
        .line1 = sgl::SoundId{.id = 4},
        .line2 = sgl::SoundId{.id = 5},
        .line3 = sgl::SoundId{.id = 6},
        .tetris = sgl::SoundId{.id = 7},
        .levelUp = sgl::SoundId{.id = 8},
        .gameOver = sgl::SoundId{.id = 9},
    };
}

} // namespace

TEST(FeedbackTest, lineClearPlaysOneSoundAndEmitsPerRow)
{
    sgl::AudioSpy audio;
    const auto ids = makeIds();
    sgl::tetris::Feedback feedback{audio, ids, sgl::Pcg32{1}};

    const sgl::tetris::TetrisEvent event = sgl::tetris::LinesCleared{.count = 2};
    feedback.onEvents(std::span{&event, 1});

    ASSERT_EQ(audio.plays.size(), 1u);
    EXPECT_EQ(audio.plays[0].id, ids.line2);
    EXPECT_EQ(feedback.particleCount(), 2u * sgl::tetris::Feedback::kParticlesPerRow);
}

TEST(FeedbackTest, tetrisUsesTetrisSound)
{
    sgl::AudioSpy audio;
    const auto ids = makeIds();
    sgl::tetris::Feedback feedback{audio, ids, sgl::Pcg32{2}};

    const sgl::tetris::TetrisEvent event = sgl::tetris::LinesCleared{.count = 4};
    feedback.onEvents(std::span{&event, 1});

    ASSERT_EQ(audio.plays.size(), 1u);
    EXPECT_EQ(audio.plays[0].id, ids.tetris);
    EXPECT_EQ(feedback.particleCount(), 4u * sgl::tetris::Feedback::kParticlesPerRow);
}

TEST(FeedbackTest, shiftPlaysMove)
{
    sgl::AudioSpy audio;
    const auto ids = makeIds();
    sgl::tetris::Feedback feedback{audio, ids, sgl::Pcg32{3}};

    const sgl::tetris::TetrisEvent event = sgl::tetris::PieceShifted{};
    feedback.onEvents(std::span{&event, 1});

    ASSERT_EQ(audio.plays.size(), 1u);
    EXPECT_EQ(audio.plays[0].id, ids.move);
    EXPECT_EQ(feedback.particleCount(), 0u);
}

TEST(FeedbackTest, particlesExpire)
{
    sgl::AudioSpy audio;
    const auto ids = makeIds();
    sgl::tetris::Feedback feedback{audio, ids, sgl::Pcg32{4}};

    const sgl::tetris::TetrisEvent event = sgl::tetris::LinesCleared{.count = 1};
    feedback.onEvents(std::span{&event, 1});
    ASSERT_GT(feedback.particleCount(), 0u);

    feedback.update(sgl::Seconds{1.f});
    EXPECT_EQ(feedback.particleCount(), 0u);
}
