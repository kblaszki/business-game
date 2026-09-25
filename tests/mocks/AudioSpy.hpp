#pragma once

#include <sgl/audio/AudioI.hpp>
#include <utility>
#include <vector>

namespace sgl
{

class AudioSpy : public AudioI
{
public:
    void play(SoundId id, float volume) override
    {
        plays.push_back({id, volume});
    }

    struct Play
    {
        SoundId id{};
        float volume{};
    };

    std::vector<Play> plays;
};

} // namespace sgl
