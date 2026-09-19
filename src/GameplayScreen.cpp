/* Created by kblaszki */

#include "GameplayScreen.hpp"

#include "MainMenuScreen.hpp"
#include "ScreenStack.hpp"

#include <Ball.hpp>
#include <Brick.hpp>
#include <LevelDescriptor.hpp>
#include <Paddle.hpp>
#include <RectCollision.hpp>
#include <makeWorld.hpp>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <memory>

GameplayScreen::GameplayScreen(ScreenStack& stack, LevelId levelId)
    : m_stack(stack)
    , m_world(makeWorld(levelDescriptor(levelId)))
{
    bindObjects();
}

void GameplayScreen::bindObjects()
{
    for(std::size_t i = 0; i < m_world.objectCount(); ++i)
    {
        if(auto* const paddle = dynamic_cast<Paddle*>(m_world.objectAt(i)))
        {
            m_paddle = paddle;
        }
        else if(auto* const ball = dynamic_cast<Ball*>(m_world.objectAt(i)))
        {
            m_ball = ball;
        }
    }
}

bool GameplayScreen::handleEvent(const sf::Event& event)
{
    if(m_paddle == nullptr)
    {
        return false;
    }

    if(const auto* const pressed = event.getIf<sf::Event::KeyPressed>())
    {
        if(pressed->code == sf::Keyboard::Key::Left)
        {
            m_paddle->setMoveLeft(true);
            return true;
        }
        if(pressed->code == sf::Keyboard::Key::Right)
        {
            m_paddle->setMoveRight(true);
            return true;
        }
    }

    if(const auto* const released = event.getIf<sf::Event::KeyReleased>())
    {
        if(released->code == sf::Keyboard::Key::Left)
        {
            m_paddle->setMoveLeft(false);
            return true;
        }
        if(released->code == sf::Keyboard::Key::Right)
        {
            m_paddle->setMoveRight(false);
            return true;
        }
    }

    return false;
}

bool GameplayScreen::handleAction(Action action)
{
    ++m_handleActionCount;
    if(action == Action::Pause)
    {
        m_stack.requestPauseOverlay();
        return true;
    }

    return false;
}

void GameplayScreen::update(sf::Time dt)
{
    if(m_ending)
    {
        return;
    }

    m_world.fixedUpdate(dt);
    ++m_tickCount;

    if(m_ball != nullptr && m_ball->lost())
    {
        loseLife();
        return;
    }

    handlePaddleCollision();
    handleBrickCollisions();

    if(remainingBricks() == 0)
    {
        returnToMenu();
    }
}

void GameplayScreen::draw(sf::RenderTarget& target)
{
    ++m_drawCount;
    m_world.draw(target);
    drawLives(target);
}

void GameplayScreen::drawLives(sf::RenderTarget& target) const
{
    for(int i = 0; i < m_lives; ++i)
    {
        sf::RectangleShape pip{{16.f, 16.f}};
        pip.setPosition({16.f + static_cast<float>(i) * 20.f, 12.f});
        pip.setFillColor(sf::Color::White);
        target.draw(pip);
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

std::uint32_t GameplayScreen::tickCount() const
{
    return m_tickCount;
}

std::uint32_t GameplayScreen::drawCount() const
{
    return m_drawCount;
}

std::uint32_t GameplayScreen::handleActionCount() const
{
    return m_handleActionCount;
}

int GameplayScreen::lives() const
{
    return m_lives;
}

std::size_t GameplayScreen::remainingBricks() const
{
    std::size_t count = 0;
    for(std::size_t i = 0; i < m_world.objectCount(); ++i)
    {
        if(const auto* const brick = dynamic_cast<const Brick*>(m_world.objectAt(i)); brick != nullptr && brick->alive())
        {
            ++count;
        }
    }
    return count;
}

sf::Vector2f GameplayScreen::paddlePosition() const
{
    return m_paddle != nullptr ? m_paddle->position() : sf::Vector2f{};
}

sf::Vector2f GameplayScreen::ballPosition() const
{
    return m_ball != nullptr ? m_ball->position() : sf::Vector2f{};
}

void GameplayScreen::destroyAllBricksForTest()
{
    for(std::size_t i = 0; i < m_world.objectCount(); ++i)
    {
        if(auto* const brick = dynamic_cast<Brick*>(m_world.objectAt(i)))
        {
            brick->destroy();
        }
    }
}

void GameplayScreen::loseLifeForTest()
{
    loseLife();
}

void GameplayScreen::dropBallForTest()
{
    if(m_ball == nullptr)
    {
        return;
    }

    m_ball->setPosition({640.f, 720.f});
    m_ball->setVelocity({0.f, 420.f});
}

void GameplayScreen::handlePaddleCollision()
{
    if(m_ball == nullptr || m_paddle == nullptr || m_ball->velocity().y <= 0.f)
    {
        return;
    }

    const sf::FloatRect ballBounds = m_ball->bounds();
    const sf::FloatRect paddleBounds = m_paddle->bounds();
    if(!intersects(ballBounds, paddleBounds))
    {
        return;
    }

    sf::Vector2f velocity = m_ball->velocity();
    bounceBallOffPaddle(velocity, ballBounds, paddleBounds);
    m_ball->setVelocity(velocity);

    sf::Vector2f pos = m_ball->position();
    pos.y = paddleBounds.position.y - Ball::RADIUS - 1.f;
    m_ball->setPosition(pos);
}

void GameplayScreen::handleBrickCollisions()
{
    if(m_ball == nullptr)
    {
        return;
    }

    const sf::FloatRect ballBounds = m_ball->bounds();
    for(std::size_t i = 0; i < m_world.objectCount(); ++i)
    {
        auto* const brick = dynamic_cast<Brick*>(m_world.objectAt(i));
        if(brick == nullptr || !brick->alive() || !intersects(ballBounds, brick->bounds()))
        {
            continue;
        }

        sf::Vector2f velocity = m_ball->velocity();
        bounceBallOffAabb(velocity, ballBounds, brick->bounds());
        m_ball->setVelocity(velocity);
        brick->destroy();
        break;
    }
}

void GameplayScreen::loseLife()
{
    --m_lives;
    if(m_lives <= 0)
    {
        returnToMenu();
        return;
    }

    if(m_ball != nullptr && m_paddle != nullptr)
    {
        m_ball->resetAbove(*m_paddle);
    }
}

void GameplayScreen::returnToMenu()
{
    if(m_ending)
    {
        return;
    }

    m_ending = true;
    m_stack.requestReplace(std::make_unique<MainMenuScreen>(m_stack));
}
