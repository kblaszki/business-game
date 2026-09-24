#pragma once

#include <cstddef>
#include <eng/render/DrawCommand.hpp>
#include <span>
#include <vector>

namespace eng
{

class RenderQueue
{
public:
    void push(Layer layer, float depth, DrawCommand command);
    std::span<const Entry> sorted();
    std::size_t size() const;
    void clear();

private:
    std::vector<Entry> entries_;
    std::uint32_t nextSeq_{};
    bool needsSort_{};
};

} // namespace eng
