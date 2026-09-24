#pragma once

#include <eng/core/Time.hpp>

namespace eng
{

class ClockI
{
public:
    virtual ~ClockI() = default;

    virtual Seconds restart() = 0;
};

} // namespace eng
