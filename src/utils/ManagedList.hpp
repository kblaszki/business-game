/* Created by kblaszki */
#pragma once

#include <functional>
#include <list>
#include <utility>

template<typename T>
class ManagedList
{
public:
    class UnRegisterer
    {
    public:
        UnRegisterer() = default;

        explicit UnRegisterer(std::function<void()> eraser)
            : eraser{std::move(eraser)}
        {
        }

        ~UnRegisterer()
        {
            if(eraser)
            {
                eraser();
            }
        }

        UnRegisterer(UnRegisterer&& other) noexcept
            : eraser{std::move(other.eraser)}
        {
            other.eraser = nullptr;
        }

        UnRegisterer& operator=(UnRegisterer&& other) noexcept
        {
            if(this != &other)
            {
                if(eraser)
                {
                    eraser();
                }
                eraser = std::move(other.eraser);
                other.eraser = nullptr;
            }
            return *this;
        }

        UnRegisterer(const UnRegisterer&) = delete;
        UnRegisterer& operator=(const UnRegisterer&) = delete;

    private:
        std::function<void()> eraser;
    };

    using iterator = typename std::list<T>::iterator;
    using const_iterator = typename std::list<T>::const_iterator;

    [[nodiscard]] UnRegisterer emplace(T&& value)
    {
        auto it = elements.emplace(elements.end(), std::forward<T>(value));
        return UnRegisterer{[this, it]() { elements.erase(it); }};
    }

    iterator begin()
    {
        return elements.begin();
    }

    const_iterator begin() const
    {
        return elements.begin();
    }

    iterator end()
    {
        return elements.end();
    }

    const_iterator end() const
    {
        return elements.end();
    }

private:
    std::list<T> elements{};
};
