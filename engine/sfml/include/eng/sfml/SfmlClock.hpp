#pragma once

#include <SFML/System/Clock.hpp>

#include <eng/loop/ClockI.hpp>

namespace eng::sfml
{

class SfmlClock : public eng::ClockI
{
public:
    eng::Seconds restart() override;

private:
    sf::Clock clock_{};
};

} // namespace eng::sfml
