#include <algorithm>
#include <sgl/sfml/SfmlAudio.hpp>

namespace sgl::sfml
{
namespace
{

std::expected<sf::SoundBuffer, sgl::ResourceError> loadBuffer(const sgl::Pcm& pcm)
{
    if(pcm.sampleRate == 0 || pcm.samples.empty())
    {
        return std::unexpected(sgl::ResourceError::DecodeFailed);
    }

    sf::SoundBuffer buffer;
    if(!buffer.loadFromSamples(pcm.samples.data(),
                               static_cast<std::uint64_t>(pcm.samples.size()),
                               1u,
                               pcm.sampleRate,
                               {sf::SoundChannel::Mono}))
    {
        return std::unexpected(sgl::ResourceError::DecodeFailed);
    }
    return buffer;
}

} // namespace

std::expected<sgl::SoundId, sgl::ResourceError> SfmlAudio::upload(std::string_view key, const sgl::Pcm& pcm)
{
    return buffers_.load(key, [&]() { return loadBuffer(pcm); });
}

void SfmlAudio::play(sgl::SoundId id, float volume)
{
    const sf::SoundBuffer* buffer = buffers_.get(id);
    if(buffer == nullptr)
    {
        return;
    }

    const float sfVolume = std::clamp(volume, 0.f, 1.f) * 100.f;

    std::size_t slot = kVoiceCount;
    for(std::size_t i = 0; i < kVoiceCount; ++i)
    {
        if(!voices_[i].has_value() || voices_[i]->getStatus() == sf::SoundSource::Status::Stopped)
        {
            slot = i;
            break;
        }
    }

    if(slot == kVoiceCount)
    {
        slot = 0;
        std::uint64_t oldest = voiceStarted_[0];
        for(std::size_t i = 1; i < kVoiceCount; ++i)
        {
            if(voiceStarted_[i] < oldest)
            {
                oldest = voiceStarted_[i];
                slot = i;
            }
        }
    }

    voices_[slot].emplace(*buffer);
    voices_[slot]->setVolume(sfVolume);
    voices_[slot]->play();
    voiceStarted_[slot] = ++startCounter_;
}

} // namespace sgl::sfml
