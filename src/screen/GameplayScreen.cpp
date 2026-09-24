#include "GameplayScreen.hpp"

#include <Game.hpp>
#include <memory>
#include <screen/MainMenuScreen.hpp>
#include <screen/ScreenStack.hpp>
#include <screen/UiFont.hpp>
#include <string>
#include <world/LevelDescriptor.hpp>

namespace
{
constexpr float paddleSpeed{480.f};

void placeHud(sf::Text& text, float x, float y)
{
    text.setPosition({x, y});
    text.setFillColor(sf::Color{240, 230, 210});
}
} // namespace

GameplayScreen::GameplayScreen(ScreenStack& screens, LevelId id)
    : screens{screens}
    , level{id}
    , simulated{makeWorld(levelDescriptor(id))}
    , font{makeUiFont()}
    , scoreLabel{font, "Score 0", 22}
    , livesLabel{font, "Lives 3", 22}
    , banner{{640.f, 180.f}}
    , bannerTitle{font, "", 40}
    , bannerHint{font, "Enter - retry   Backspace - menu", 20}
{
    placeHud(scoreLabel, 32.f, 16.f);
    placeHud(livesLabel, 1040.f, 16.f);
    banner.setFillColor(sf::Color{10, 10, 18, 210});
    banner.setPosition({320.f, 260.f});
    bannerTitle.setFillColor(sf::Color{255, 230, 140});
    bannerHint.setFillColor(sf::Color{230, 220, 200});
    refreshHud();
}

void GameplayScreen::applyPaddleInput()
{
    float speed{0.f};
    if(leftHeld)
    {
        speed -= paddleSpeed;
    }
    if(rightHeld)
    {
        speed += paddleSpeed;
    }
    simulated.setPaddleSpeed(speed);
}

void GameplayScreen::refreshHud()
{
    scoreLabel.setString("Score " + std::to_string(simulated.score()));
    livesLabel.setString("Lives " + std::to_string(simulated.lives()));

    if(simulated.won())
    {
        bannerTitle.setString("You win");
    }
    else if(simulated.lost())
    {
        bannerTitle.setString("You lose");
    }

    if(simulated.won() || simulated.lost())
    {
        const auto titleBounds = bannerTitle.getLocalBounds();
        bannerTitle.setOrigin(
            {titleBounds.position.x + titleBounds.size.x / 2.f, titleBounds.position.y + titleBounds.size.y / 2.f});
        bannerTitle.setPosition({640.f, 320.f});
        const auto hintBounds = bannerHint.getLocalBounds();
        bannerHint.setOrigin(
            {hintBounds.position.x + hintBounds.size.x / 2.f, hintBounds.position.y + hintBounds.size.y / 2.f});
        bannerHint.setPosition({640.f, 380.f});
    }
}

void GameplayScreen::restart()
{
    simulated = makeWorld(levelDescriptor(level));
    leftHeld = false;
    rightHeld = false;
    applyPaddleInput();
    refreshHud();
}

bool GameplayScreen::handleEvent(const sf::Event& event)
{
    if(const auto* pressed = event.getIf<sf::Event::KeyPressed>())
    {
        if(pressed->code == sf::Keyboard::Key::Left || pressed->code == sf::Keyboard::Key::A)
        {
            leftHeld = true;
            applyPaddleInput();
            return true;
        }
        if(pressed->code == sf::Keyboard::Key::Right || pressed->code == sf::Keyboard::Key::D)
        {
            rightHeld = true;
            applyPaddleInput();
            return true;
        }
    }

    if(const auto* released = event.getIf<sf::Event::KeyReleased>())
    {
        if(released->code == sf::Keyboard::Key::Left || released->code == sf::Keyboard::Key::A)
        {
            leftHeld = false;
            applyPaddleInput();
            return true;
        }
        if(released->code == sf::Keyboard::Key::Right || released->code == sf::Keyboard::Key::D)
        {
            rightHeld = false;
            applyPaddleInput();
            return true;
        }
    }

    return false;
}

bool GameplayScreen::handleAction(Action action)
{
    if(simulated.won() || simulated.lost())
    {
        if(action == Action::Confirm)
        {
            restart();
            return true;
        }
        if(action == Action::Cancel)
        {
            screens.replace(std::make_unique<MainMenuScreen>(screens));
            return true;
        }
        return false;
    }

    if(action == Action::Pause)
    {
        screens.requestPauseOverlay();
        return true;
    }

    if(action == Action::Confirm)
    {
        simulated.launch();
        return true;
    }

    return false;
}

void GameplayScreen::update(sf::Time dt)
{
    simulated.fixedUpdate(dt);
    refreshHud();
}

void GameplayScreen::draw(DrawerI& drawer)
{
    simulated.draw(drawer);
    drawer.draw(scoreLabel);
    drawer.draw(livesLabel);
    if(simulated.won() || simulated.lost())
    {
        drawer.draw(banner);
        drawer.draw(bannerTitle);
        drawer.draw(bannerHint);
    }
}

bool GameplayScreen::blocksUpdate() const
{
    return true;
}

bool GameplayScreen::blocksDraw() const
{
    return true;
}

bool GameplayScreen::acceptsPauseOverlay() const
{
    return !simulated.won() && !simulated.lost();
}

std::uint32_t GameplayScreen::tickCount() const
{
    return simulated.tickCount();
}

const World& GameplayScreen::world() const
{
    return simulated;
}
