#pragma once

#include <sgl/core/Time.hpp>

namespace sgl
{

struct Envelope
{
    Seconds attack{};
    Seconds release{};
};

} // namespace sgl
