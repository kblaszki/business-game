/* Created by kblaszki */

#include "GameScreen.hpp"

#include "MenuScreen.hpp"

#include <utils/RectCollision.hpp>

#include <string>

namespace
{
constexpr int BRICK_COLUMNS = 10;
constexpr int BRICK_ROWS = 5;
constexpr float BRICK_GAP = 4.f;
constexpr float BRICK_TOP = 60.f;

sf::Color brickColorForRow(int row)
{
    static constexpr sf::Color colors[] = {
        sf::Color{220, 80, 80},
        sf::Color{220, 160, 60},
        sf::Color{220, 220, 80},
        sf::Color{80, 180, 80},
        sf::Color{80, 140, 220},
    };
    return colors[row % 5];
}
} // namespace

GameScreen::GameScreen(EventManagers& eventManagers,
                       ScreenRendererI& screenRenderer,
                       ScreenUpdaterI& screenUpdater,
                       ResourceManager& resources)
    : eventManagers{eventManagers}
    , screenRenderer{screenRenderer}
    , screenUpdater{screenUpdater}
    , resources{resources}
    , gamePaddle{eventManagers.get<ManagerOf::Keyboard>()}
    , livesText{resources.getFont("fonts/prototype.ttf")}
{
    livesText.setCharacterSize(24);
    livesText.setFillColor(sf::Color::White);
    livesText.setPosition({16.f, 12.f});
    updateHud();

    buildBricks();
    gameBall.resetAbove(gamePaddle.getPosition(), gamePaddle.getSize());
}

void GameScreen::buildBricks()
{
    bricks.clear();
    const float totalWidth = BRICK_COLUMNS * Brick::WIDTH + (BRICK_COLUMNS - 1) * BRICK_GAP;
    const float startX = (ARENA_WIDTH - totalWidth) / 2.f;
    for(int row = 0; row < BRICK_ROWS; ++row)
    {
        for(int col = 0; col < BRICK_COLUMNS; ++col)
        {
            const float x = startX + static_cast<float>(col) * (Brick::WIDTH + BRICK_GAP);
            const float y = BRICK_TOP + static_cast<float>(row) * (Brick::HEIGHT + BRICK_GAP);
            bricks.emplace_back(sf::Vector2f{x, y}, brickColorForRow(row));
        }
    }
}

void GameScreen::updateHud()
{
    livesText.setString("Lives: " + std::to_string(livesRemaining));
}

void GameScreen::update(float dt)
{
    if(transitioning)
    {
        return;
    }

    gamePaddle.update(dt);
    gameBall.update(dt);

    handleWallCollisions();
    if(transitioning)
    {
        return;
    }

    handlePaddleCollision();
    handleBrickCollisions();

    if(remainingBricks() == 0)
    {
        returnToMenu();
    }
}

void GameScreen::display()
{
    for(const auto& brick: bricks)
    {
        brick.draw(screenRenderer);
    }
    gamePaddle.draw(screenRenderer);
    gameBall.draw(screenRenderer);
    screenRenderer.draw(livesText);
}

int GameScreen::lives() const
{
    return livesRemaining;
}

std::size_t GameScreen::remainingBricks() const
{
    std::size_t count = 0;
    for(const auto& brick: bricks)
    {
        if(not brick.isDestroyed())
        {
            ++count;
        }
    }
    return count;
}

std::optional<sf::FloatRect> GameScreen::firstAliveBrickBounds() const
{
    for(const auto& brick: bricks)
    {
        if(not brick.isDestroyed())
        {
            return brick.getBounds();
        }
    }
    return std::nullopt;
}

Ball& GameScreen::ball()
{
    return gameBall;
}

const Paddle& GameScreen::paddle() const
{
    return gamePaddle;
}

void GameScreen::handleWallCollisions()
{
    const sf::FloatRect bounds = gameBall.getBounds();
    sf::Vector2f velocity = gameBall.getVelocity();
    sf::Vector2f position = gameBall.getPosition();
    bool changed = false;

    if(bounds.position.x <= 0.f)
    {
        bounceBallOffVerticalWall(velocity);
        position.x = Ball::RADIUS;
        changed = true;
    }
    else if(bounds.position.x + bounds.size.x >= ARENA_WIDTH)
    {
        bounceBallOffVerticalWall(velocity);
        position.x = ARENA_WIDTH - Ball::RADIUS;
        changed = true;
    }

    if(bounds.position.y <= 0.f)
    {
        bounceBallOffHorizontalWall(velocity);
        position.y = Ball::RADIUS;
        changed = true;
    }
    else if(bounds.position.y + bounds.size.y >= ARENA_HEIGHT)
    {
        loseLife();
        return;
    }

    if(changed)
    {
        gameBall.setVelocity(velocity);
        gameBall.setPosition(position);
    }
}

void GameScreen::handlePaddleCollision()
{
    if(gameBall.getVelocity().y <= 0.f)
    {
        return;
    }

    const sf::FloatRect ballBounds = gameBall.getBounds();
    const sf::FloatRect paddleBounds = gamePaddle.getBounds();
    if(not intersects(ballBounds, paddleBounds))
    {
        return;
    }

    sf::Vector2f velocity = gameBall.getVelocity();
    bounceBallOffPaddle(velocity, ballBounds, paddleBounds);
    gameBall.setVelocity(velocity);

    sf::Vector2f position = gameBall.getPosition();
    position.y = paddleBounds.position.y - Ball::RADIUS - 1.f;
    gameBall.setPosition(position);
}

void GameScreen::handleBrickCollisions()
{
    const sf::FloatRect ballBounds = gameBall.getBounds();
    for(auto& brick: bricks)
    {
        if(brick.isDestroyed())
        {
            continue;
        }
        if(not intersects(ballBounds, brick.getBounds()))
        {
            continue;
        }

        sf::Vector2f velocity = gameBall.getVelocity();
        bounceBallOffAabb(velocity, ballBounds, brick.getBounds());
        gameBall.setVelocity(velocity);
        brick.destroy();
        break;
    }
}

void GameScreen::loseLife()
{
    --livesRemaining;
    updateHud();
    if(livesRemaining <= 0)
    {
        returnToMenu();
        return;
    }
    gameBall.resetAbove(gamePaddle.getPosition(), gamePaddle.getSize());
}

void GameScreen::returnToMenu()
{
    if(transitioning)
    {
        return;
    }
    transitioning = true;
    screenUpdater.replaceScreen(std::make_unique<MenuScreen>(eventManagers, screenRenderer, screenUpdater, resources));
}
