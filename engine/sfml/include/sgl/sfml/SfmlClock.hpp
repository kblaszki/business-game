#pragma once

#include <SFML/System/Clock.hpp>

#include <sgl/loop/ClockI.hpp>

namespace sgl::sfml
{

class SfmlClock : public sgl::ClockI
{
public:
    sgl::Seconds restart() override;

private:
    sf::Clock clock_{};
};

} // namespace sgl::sfml
