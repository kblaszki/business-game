#pragma once

#include <window/DrawerI.hpp>

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

#include <cstddef>
#include <memory>
#include <screen/ScreenI.hpp>
#include <vector>

class ScreenStack
{
public:
    ScreenStack() = default;
    ScreenStack(const ScreenStack&) = delete;
    ScreenStack& operator=(const ScreenStack&) = delete;

    void push(std::unique_ptr<ScreenI> screen);
    void pop();
    void replace(std::unique_ptr<ScreenI> screen);

    bool handleEvent(const sf::Event& event);
    void update(sf::Time dt);
    void draw(DrawerI& drawer);

    [[nodiscard]] bool empty() const;
    [[nodiscard]] std::size_t size() const;
    [[nodiscard]] bool blocksUpdate() const;
    [[nodiscard]] ScreenI* top() const;

private:
    enum class CommandType
    {
        Push,
        Pop,
        Replace
    };

    struct Command
    {
        CommandType type{CommandType::Pop};
        std::unique_ptr<ScreenI> screen{};
    };

    void applyCommands();
    void enqueue(Command command);

    std::vector<std::unique_ptr<ScreenI>> screens;
    std::vector<Command> commands;
    bool dispatching{false};
};
