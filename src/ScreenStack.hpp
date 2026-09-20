/* Created by kblaszki */
#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

#include <Action.hpp>
#include <IScreen.hpp>
#include <cstddef>
#include <memory>
#include <vector>

class ScreenStack
{
public:
    ScreenStack() = default;
    ScreenStack(const ScreenStack&) = delete;
    ScreenStack& operator=(const ScreenStack&) = delete;

    void requestPush(std::unique_ptr<IScreen> screen);
    void requestPop();
    void requestReplace(std::unique_ptr<IScreen> screen);
    void requestClose();
    void requestPauseOverlay();
    void applyCommands();

    bool handleEvent(const sf::Event& event);
    bool handleAction(Action action);
    void update(sf::Time dt);
    void draw(sf::RenderTarget& target);

    [[nodiscard]] bool empty() const;
    [[nodiscard]] std::size_t size() const;
    [[nodiscard]] bool closeRequested() const;
    [[nodiscard]] IScreen* top() const;
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
        std::unique_ptr<IScreen> screen{};
    };

    std::vector<std::unique_ptr<IScreen>> m_screens;
    std::vector<Command> m_commands;
    bool m_closeRequested{false};
};
