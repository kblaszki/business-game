#include "MainMenuScreen.hpp"

#include <Game.hpp>
#include <memory>
#include <screen/GameplayScreen.hpp>
#include <screen/ScreenStack.hpp>
#include <screen/UiFont.hpp>

namespace
{
constexpr sf::Vector2f buttonSize{400.f, 72.f};

void centerAt(sf::Text& text, float y)
{
    const auto bounds = text.getLocalBounds();
    text.setOrigin({bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f});
    text.setPosition({static_cast<float>(Game::DESIGN_SIZE.x) / 2.f, y});
}

void centerOn(sf::Text& text, const sf::RectangleShape& button)
{
    const auto box = button.getGlobalBounds();
    const auto bounds = text.getLocalBounds();
    text.setOrigin({bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f});
    text.setPosition({box.position.x + box.size.x / 2.f, box.position.y + box.size.y / 2.f});
}

bool contains(const sf::RectangleShape& button, sf::Vector2f point)
{
    return button.getGlobalBounds().contains(point);
}
} // namespace

MainMenuScreen::MainMenuScreen(ScreenStack& screens)
    : screens{screens}
    , font{makeUiFont()}
    , startButton{buttonSize}
    , quitButton{buttonSize}
    , title{font, "Breakout", 56}
    , startLabel{font, "Start", 28}
    , quitLabel{font, "Quit", 28}
{
    const float x = (static_cast<float>(Game::DESIGN_SIZE.x) - buttonSize.x) / 2.f;
    startButton.setPosition({x, 320.f});
    quitButton.setPosition({x, 420.f});
    title.setFillColor(sf::Color{255, 230, 160});
    startLabel.setFillColor(sf::Color{20, 16, 12});
    quitLabel.setFillColor(sf::Color{255, 230, 230});
    centerAt(title, 200.f);
    paintButtons();
}

void MainMenuScreen::startGame()
{
    screens.replace(std::make_unique<GameplayScreen>(screens, LevelId::Stage1));
}

void MainMenuScreen::paintButtons()
{
    startButton.setFillColor(startHover ? sf::Color{255, 210, 80} : sf::Color{230, 180, 50});
    quitButton.setFillColor(quitHover ? sf::Color{180, 60, 70} : sf::Color{140, 40, 50});
    centerOn(startLabel, startButton);
    centerOn(quitLabel, quitButton);
}

bool MainMenuScreen::hitStart(sf::Vector2f point) const
{
    return contains(startButton, point);
}

bool MainMenuScreen::hitQuit(sf::Vector2f point) const
{
    return contains(quitButton, point);
}

sf::FloatRect MainMenuScreen::startButtonBounds() const
{
    return startButton.getGlobalBounds();
}

bool MainMenuScreen::handleEvent(const sf::Event& event)
{
    if(const auto* move = event.getIf<sf::Event::MouseMoved>())
    {
        const sf::Vector2f point{static_cast<float>(move->position.x), static_cast<float>(move->position.y)};
        startHover = hitStart(point);
        quitHover = hitQuit(point);
        paintButtons();
        return true;
    }

    if(const auto* click = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if(click->button != sf::Mouse::Button::Left)
        {
            return false;
        }

        const sf::Vector2f point{static_cast<float>(click->position.x), static_cast<float>(click->position.y)};
        if(hitStart(point))
        {
            startGame();
            return true;
        }
        if(hitQuit(point))
        {
            screens.requestClose();
            return true;
        }
    }

    return false;
}

bool MainMenuScreen::handleAction(Action action)
{
    if(action == Action::Confirm)
    {
        startGame();
        return true;
    }

    if(action == Action::Cancel)
    {
        screens.requestClose();
        return true;
    }

    return false;
}

void MainMenuScreen::update(sf::Time) {}

void MainMenuScreen::draw(DrawerI& drawer)
{
    drawer.draw(art.background);
    drawer.draw(title);
    drawer.draw(startButton);
    drawer.draw(startLabel);
    drawer.draw(quitButton);
    drawer.draw(quitLabel);
}

bool MainMenuScreen::blocksUpdate() const
{
    return true;
}

bool MainMenuScreen::blocksDraw() const
{
    return true;
}
