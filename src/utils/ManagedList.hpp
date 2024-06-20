#include <functional>
#include <iostream>
#include <list>

template<typename T>
class ManagedList
{
public:
    using UnRegisterer = std::function<void()>;
    using iterator = typename std::list<T>::iterator;
    using const_iterator = typename std::list<T>::const_iterator;

    UnRegisterer add(T&& value)
    {
        auto it = elements.emplace_back(std::forward<T>(value));
        return [this, it]() { elements.erase(it); };
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
    std::list<T> elements;
};