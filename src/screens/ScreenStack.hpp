/* Created by kblaszki */
#pragma once

#include "ScreenI.hpp"
#include "ScreenUpdaterI.hpp"

#include <window/DrawerI.hpp>

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

#include <Action.hpp>
#include <cstddef>
#include <memory>
#include <vector>

class ScreenStack : public ScreenUpdaterI
{
public:
    ScreenStack() = default;
    ScreenStack(const ScreenStack&) = delete;
    ScreenStack& operator=(const ScreenStack&) = delete;

    void requestPush(std::unique_ptr<ScreenI> screen) override;
    void requestPop() override;
    void requestReplace(std::unique_ptr<ScreenI> screen) override;
    void requestClose() override;
    void requestPauseOverlay() override;
    void applyCommands();

    bool handleEvent(const sf::Event& event);
    bool handleAction(Action action);
    void update(sf::Time dt);
    void draw(DrawerI& drawer);

    [[nodiscard]] bool empty() const;
    [[nodiscard]] std::size_t size() const;
    [[nodiscard]] bool closeRequested() const;
    [[nodiscard]] ScreenI* top() const;
    [[nodiscard]] bool gameplayIsTop() const;
    [[nodiscard]] bool pauseIsTop() const;
    [[nodiscard]] std::size_t drawStartIndex() const;

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

    std::vector<std::unique_ptr<ScreenI>> m_screens;
    std::vector<Command> m_commands;
    bool m_closeRequested{false};
};
