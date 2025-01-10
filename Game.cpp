#include "Game.h"
#include <fstream>
#include <iostream>

void Game::init(const std::string& configFile)
{
	std::cout << "Game initialized";
}

void Game::run()
{
}

void Game::setPaused(bool b)
{
}

void Game::sMovement()
{
}

void Game::sUserInput()
{
}

void Game::sLifeSpan()
{
}

void Game::sRender()
{
}

void Game::sEnemySpawner()
{
}

void Game::sCollision()
{
}

void Game::spawnPlayer()
{
}

void Game::spawnEnemy()
{
}

void Game::spawnSmallEnemies(std::shared_ptr<Entity> e)
{
}

void Game::spawnBullet(std::shared_ptr<Entity> player, const vec2& mousePos)
{
}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity)
{
}
