#pragma once

namespace sgl
{

template<class... Ts>
struct Overloaded : Ts...
{
    using Ts::operator()...;
};

} // namespace sgl
