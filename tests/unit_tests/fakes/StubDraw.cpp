/* Created by kblaszki */

#include <Ball.hpp>
#include <Brick.hpp>
#include <GameplayScreen.hpp>
#include <MainMenuScreen.hpp>
#include <Paddle.hpp>
#include <PauseScreen.hpp>
#include <World.hpp>

void Paddle::draw(sf::RenderTarget&) const {}

void Ball::draw(sf::RenderTarget&) const {}

void Brick::draw(sf::RenderTarget&) const {}

void World::draw(sf::RenderTarget&) const {}

void GameplayScreen::draw(sf::RenderTarget&)
{
    ++m_drawCount;
}

void GameplayScreen::drawLives(sf::RenderTarget&) const {}

void PauseScreen::draw(sf::RenderTarget&) {}

void MainMenuScreen::draw(sf::RenderTarget&) {}
