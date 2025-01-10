#include "Game.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <SFML/Graphics.hpp>
#include "Components.h"

void Game::init(const std::string& configFile){
	
	std::ifstream iFile (configFile);

    if (!iFile.is_open())
    {
        std::cerr << "Failed to open config file: " << configFile << std::endl;
        return;
    }

    std::string temp;

    // window setup

    int window_width, window_height, fps, is_fullscreen;
    iFile >> temp >> window_width >> window_height >> fps >> is_fullscreen;
    m_window.create(sf::VideoMode(window_width, window_height), "Geometry Boom Game");
    m_window.setFramerateLimit(fps);

    //std::cout << temp << ' ' << window_width << ' ' << window_height << ' ' << fps << ' ' << is_fullscreen;


    // font and text setup

    std::string fontFile;
    int fontSize, R, G, B;

    iFile >> temp >> fontFile >> fontSize >> R >> G >> B;

    m_font.loadFromFile(fontFile);
    m_scoreText.setString("0");
    m_scoreText.setFont(m_font);
    m_scoreText.setCharacterSize(fontSize);
    m_scoreText.setFillColor(sf::Color(R, G, B));

    if (iFile.fail())
    {
        std::cerr << "Error reading configuration data" << std::endl;
        return;
    }

    // Player config setup

    iFile >> temp >> playerConfig.ShapeRadius >> playerConfig.CollisionRadius >> playerConfig.Speed >> R >> G >> B;
    //std::cout<<' '<< temp <<' '<< playerConfig.ShapeRadius <<' '<< playerConfig.CollisionRadius <<' '<< playerConfig.Speed <<' '<< R <<' '<< G <<' '<< B;
    playerConfig.FillColor = sf::Color(R, G, B);

    iFile >> R >> G >> B;
    //std::cout<<' '<< R <<' '<< G <<' '<< B;
    playerConfig.OutlineColor = sf::Color(R, G, B);
    iFile >> playerConfig.OutlineThickness >> playerConfig.ShapeVertices;
    //std::cout<<' '<< playerConfig.OutlineThickness <<' '<< playerConfig.ShapeVertices;


    // Enemy config setup

    iFile >> temp >> enemyConfig.ShapeRadius >> enemyConfig.CollisionRadius >> enemyConfig.SpeedMin >> enemyConfig.SpeedMax >> R >> G >> B;
    enemyConfig.OutlineColor = sf::Color(R, G, B);
    iFile >> enemyConfig.OutlineThickness >> enemyConfig.ShapeVerticesMin >> enemyConfig.ShapeVerticesMax >> enemyConfig.SmallPartLifespan >> enemyConfig.SpawnInterval;

    // Bullet config setup

    iFile >> temp >> bulletConfig.ShapeRadius >> bulletConfig.CollisionRadius >> bulletConfig.Speed >> R >> G >> B;
    bulletConfig.FillColor = sf::Color(R, G, B);
    iFile >> R >> G >> B;
    bulletConfig.OutlineColor = sf::Color(R, G, B);
    iFile >> bulletConfig.OutlineThickness >> bulletConfig.ShapeVertices>> bulletConfig.Lifespan;

    iFile.close();

}

void Game::run()
{
    spawnPlayer();
    sRender();
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
    //m_window.create(sf::VideoMode(1280, 720), "Geometry Boom Game");
    sf::CircleShape c = m_player.get()->cShape->circle;
    while(m_window.isOpen())
    {   
        m_window.clear();
        sf::Event event;
        while (m_window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) 
            {
                m_window.close();
                return;
            }
        }

        for (auto& e : m_eManager.getEntities())
        {
            e->cShape->circle.setPosition(e->cTransform->pos.x, e->cTransform->pos.y);
            e->cShape->circle.rotate(3);
            m_window.draw(e->cShape->circle);
        }

        //std::cout << m_player.get()->cShape->circle.getRadius() << std::endl;
        //m_window.draw(m_player->cShape->circle);
        
        m_window.display();
    }
}

void Game::sEnemySpawner()
{
}

void Game::sCollision()
{
}

void Game::spawnPlayer()
{
    
    m_player = m_eManager.addEntity("player");

    m_player->cTransform = std::make_shared<CTransform>(vec2(m_window.getSize().x / 2, m_window.getSize().y / 2), vec2(0, 0), 0);

    m_player->cShape = std::make_shared<CShape>(playerConfig.ShapeRadius, playerConfig.ShapeVertices,
        playerConfig.FillColor, playerConfig.OutlineColor, playerConfig.OutlineThickness);

    m_player->cCollision = std::make_shared<CCollision>(playerConfig.CollisionRadius);

    //std::cout << playerConfig.ShapeRadius << " " << playerConfig.ShapeVertices << " " << playerConfig.OutlineThickness;


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
