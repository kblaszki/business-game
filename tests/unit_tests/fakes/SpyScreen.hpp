/* Created by kblaszki */
#pragma once

#include <IScreen.hpp>
#include <cstdint>
#include <functional>

class SpyScreen : public IScreen
{
public:
    SpyScreen() = default;

    SpyScreen(bool blocksUpdateFlag, bool blocksDrawFlag)
        : blockUpdate(blocksUpdateFlag)
        , blockDraw(blocksDrawFlag)
    {
    }

    ~SpyScreen() override
    {
        if(destructorCount != nullptr)
        {
            ++(*destructorCount);
        }
    }

    SpyScreen(const SpyScreen&) = delete;
    SpyScreen& operator=(const SpyScreen&) = delete;

    bool handleEvent(const sf::Event&) override
    {
        ++handleEventCount;
        return consumeEvent;
    }

    bool handleAction(Action) override
    {
        ++handleActionCount;
        return consumeAction;
    }

    void update(sf::Time) override
    {
        ++updateCount;
        if(onUpdate)
        {
            onUpdate();
        }
        finishedUpdate = true;
    }

    void draw(sf::RenderTarget&) override
    {
        ++drawCount;
    }

    bool blocksUpdate() const override
    {
        return blockUpdate;
    }

    bool blocksDraw() const override
    {
        return blockDraw;
    }

    bool consumeEvent{false};
    bool consumeAction{false};
    bool blockUpdate{false};
    bool blockDraw{false};
    bool finishedUpdate{false};
    std::function<void()> onUpdate{};
    std::uint32_t* destructorCount{nullptr};

    std::uint32_t handleEventCount{0};
    std::uint32_t handleActionCount{0};
    std::uint32_t updateCount{0};
    std::uint32_t drawCount{0};
};
