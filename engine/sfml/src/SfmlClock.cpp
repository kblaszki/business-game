#include <eng/sfml/SfmlClock.hpp>

#include <SFML/System/Time.hpp>

namespace eng::sfml
{

eng::Seconds SfmlClock::restart()
{
    return eng::Seconds{clock_.restart().asSeconds()};
}

} // namespace eng::sfml
