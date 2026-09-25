#pragma once

#include <sgl/audio/AudioI.hpp>

namespace sgl
{

class NullAudio : public AudioI
{
public:
    void play(SoundId id, float volume) override;
};

} // namespace sgl
