#include <algorithm>
#include <sgl/render/RenderQueue.hpp>
#include <utility>

namespace sgl
{

void RenderQueue::push(Layer layer, float depth, DrawCommand command)
{
    entries_.push_back(Entry{
        .key = SortKey{.layer = layer, .depth = depth, .seq = nextSeq_++},
        .command = std::move(command),
    });
    needsSort_ = true;
}

std::span<const Entry> RenderQueue::sorted()
{
    if(needsSort_)
    {
        std::ranges::stable_sort(entries_, {}, &Entry::key);
        needsSort_ = false;
    }
    return entries_;
}

std::size_t RenderQueue::size() const
{
    return entries_.size();
}

void RenderQueue::clear()
{
    entries_.clear();
    nextSeq_ = 0;
    needsSort_ = false;
}

} // namespace sgl
