#include <SFML/System/Time.hpp>

#include <sgl/sfml/SfmlClock.hpp>

namespace sgl::sfml
{

sgl::Seconds SfmlClock::restart()
{
    return sgl::Seconds{clock_.restart().asSeconds()};
}

} // namespace sgl::sfml
