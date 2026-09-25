#include <tetris/sim/Bag.hpp>
#include <utility>

namespace sgl::tetris
{

Bag::Bag(std::uint64_t seed)
    : rng_{seed}
{
    ensure(5);
}

void Bag::refill()
{
    std::array<PieceType, 7> bag{
        PieceType::I, PieceType::O, PieceType::T, PieceType::S, PieceType::Z, PieceType::J, PieceType::L};
    for(int i = 6; i > 0; --i)
    {
        const auto j = static_cast<int>(rng_.uniformBelow(static_cast<std::uint32_t>(i + 1)));
        std::swap(bag[static_cast<std::size_t>(i)], bag[static_cast<std::size_t>(j)]);
    }
    queue_.insert(queue_.end(), bag.begin(), bag.end());
}

void Bag::ensure(std::size_t count)
{
    while(queue_.size() < count)
    {
        refill();
    }
}

PieceType Bag::next()
{
    ensure(1);
    const PieceType type = queue_.front();
    queue_.erase(queue_.begin());
    ensure(5);
    return type;
}

std::array<PieceType, 5> Bag::preview() const
{
    std::array<PieceType, 5> out{};
    for(std::size_t i = 0; i < out.size(); ++i)
    {
        out[i] = queue_[i];
    }
    return out;
}

} // namespace sgl::tetris
