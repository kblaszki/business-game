#pragma once

#include <sgl/audio/SoundId.hpp>

namespace sgl
{

class AudioI
{
public:
    virtual ~AudioI();
    virtual void play(SoundId id, float volume) = 0;
};

} // namespace sgl
