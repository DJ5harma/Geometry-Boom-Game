#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "vec2.h"
#include <memory>
#include "Entity.h"
#include "EntityManager.h"

struct PlayerConfig 
{
	int			ShapeRadius;
	int			CollisionRadius;
	float		Speed;
	sf::Color   FillColor;
	sf::Color   OutlineColor;
	int			OutlineThickness;
	int			ShapeVertices;
};

struct EnemyConfig 
{
	int			ShapeRadius;
	int			CollisionRadius;
	float		SpeedMin;
	float		SpeedMax;
	// random	FillColor
	sf::Color   OutlineColor;
	int			OutlineThickness;
	int			ShapeVerticesMin;
	int			ShapeVerticesMax;
	int			SpawnInterval;
	int			SmallPartLifespan;
};

struct BulletConfig 
{
	int			ShapeRadius;
	int			CollisionRadius;
	float		Speed;
	sf::Color   FillColor;
	sf::Color   OutlineColor;
	int			OutlineThickness;
	int			ShapeVertices;
	int			Lifespan;
};

class Game
{

	PlayerConfig playerConfig;
	EnemyConfig enemyConfig;
	BulletConfig bulletConfig;

	sf::RenderWindow		m_window;
	EntityManager			m_eManager;
	sf::Font				m_font;
	sf::Text				m_scoreText;
	int						m_score = 0;
	//int						m_currentFrame = 0;
	int						m_lastEnemySpawnTime = 0;
	bool					m_paused = false;
	bool					m_running = false;

	std::shared_ptr<Entity> m_player;

	void setPaused(bool b);

	// systems
	void sMovement();
	void sUserInput();
	void sLifeSpan();
	void sRender();
	void sEnemySpawner();
	void sCollision();

	void spawnPlayer();
	void spawnEnemy();
	void spawnSmallEnemies(std::shared_ptr<Entity> e);
	void spawnBullet(std::shared_ptr<Entity> player, const vec2  &mousePos);
	void spawnSpecialWeapon(std::shared_ptr<Entity> entity);

public:
	void init(const std::string& configFile);
	void run();
};

