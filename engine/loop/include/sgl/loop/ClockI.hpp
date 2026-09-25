#pragma once

#include <sgl/core/Time.hpp>

namespace sgl
{

class ClockI
{
public:
    virtual ~ClockI() = default;

    virtual Seconds restart() = 0;
};

} // namespace sgl
