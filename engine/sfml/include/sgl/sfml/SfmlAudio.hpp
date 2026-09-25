#pragma once

#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

#include <array>
#include <cstdint>
#include <expected>
#include <optional>
#include <sgl/audio/AudioI.hpp>
#include <sgl/audio/Pcm.hpp>
#include <sgl/audio/SoundId.hpp>
#include <sgl/resources/ResourceCache.hpp>
#include <sgl/resources/ResourceError.hpp>
#include <string_view>

namespace sgl::sfml
{

class SfmlAudio : public sgl::AudioI
{
public:
    static constexpr std::size_t kVoiceCount = 16;

    [[nodiscard]] std::expected<sgl::SoundId, sgl::ResourceError> upload(std::string_view key, const sgl::Pcm& pcm);

    void play(sgl::SoundId id, float volume) override;

private:
    sgl::ResourceCache<sgl::SoundId, sf::SoundBuffer> buffers_{};
    std::array<std::optional<sf::Sound>, kVoiceCount> voices_{};
    std::array<std::uint64_t, kVoiceCount> voiceStarted_{};
    std::uint64_t startCounter_{};
};

} // namespace sgl::sfml
