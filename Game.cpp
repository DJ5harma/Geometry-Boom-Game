#include "Game.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <SFML/Graphics.hpp>
#include "Components.h"

constexpr auto PI = 3.14159;

inline static int randomInt(int a, int b)
{
	static bool seeded = false;
	if (!seeded)
	{
		srand(time(0));  // Seed the random number generator once
		seeded = true;
	}

	return a + rand() % (b - a + 1);  // Generate random number between a and b (inclusive)
}
inline static float randomFloat(float a, float b)
{
	static bool seeded = false;
	if (!seeded)
	{
		srand(time(0));  // Seed the random number generator once
		seeded = true;
	}

	return a + static_cast<float>(rand()) / RAND_MAX * (b - a);  // Generate random float between a and b
}

void Game::init(const std::string& configFile) {

	std::ifstream iFile(configFile);

	if (!iFile.is_open())
	{
		std::cerr << "Failed to open config file: " << configFile << std::endl;
		return;
	}

	std::string temp;

	// window setup

	int window_width, window_height, fps, is_fullscreen;
	iFile >> temp >> window_width >> window_height >> fps >> is_fullscreen;

	if(is_fullscreen) m_window.create(sf::VideoMode::getDesktopMode(), "Geometry Boom Game", sf::Style::Fullscreen);
	else m_window.create(sf::VideoMode(window_width, window_height), "Geometry Boom Game");
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
	iFile >> bulletConfig.OutlineThickness >> bulletConfig.ShapeVertices >> bulletConfig.Lifespan;

	iFile.close();

}

void Game::run()
{
	spawnPlayer();
	std::cout << m_window.getSize().x << " " << m_window.getSize().y << std::endl;


	while (m_window.isOpen())
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
			else if (event.type == sf::Event::KeyPressed)
			{
				auto& ip = m_player->cInput;
				int key = event.key.code;
				if (key == sf::Keyboard::Up || key == sf::Keyboard::W) ip->up = true;
				if (key == sf::Keyboard::Down || key == sf::Keyboard::S) ip->down = true;
				if (key == sf::Keyboard::Left || key == sf::Keyboard::A) ip->left = true;
				if (key == sf::Keyboard::Right || key == sf::Keyboard::D) ip->right = true;
				if (key == sf::Keyboard::P) setPaused(!m_paused);
			}
			else if (event.type == sf::Event::MouseButtonPressed)
			{
				auto& ip = m_player->cInput;
				int key = event.key.code;
				if (key == sf::Mouse::Left) ip->shoot = true;
			}
		}

		if (!m_paused)
		{
			sEnemySpawner();
			sUserInput();
			sLifeSpan();
			sCollision();
		}
		sMovement();
		m_eManager.update();
		sRender();
		m_window.display();
	}
}

void Game::setPaused(bool b)
{
	//std::cout << "P pressed" << std::endl;
	m_paused = b;
}

void Game::sMovement()
{
	for (auto& e : m_eManager.getEntities())
	{
		e->cShape->circle.rotate(5);
		if (m_paused) continue;

		vec2 newPos = e->cTransform->pos + e->cTransform->velocity;

		float radius = e->cShape->circle.getRadius();

		if (newPos.x - radius < 0 || newPos.x + radius > m_window.getSize().x) e->cTransform->velocity.x *= -1;
		if (newPos.y - radius < 0 || newPos.y + radius > m_window.getSize().y) e->cTransform->velocity.y *= -1;

		e->cTransform->pos = newPos;

		e->cShape->circle.setPosition(newPos.x, newPos.y);

	}
}

void Game::sUserInput()
{
	auto& ip = m_player->cInput;
	auto& vel = m_player->cTransform->velocity;
	auto& pos = m_player->cTransform->pos;

	if (ip->up)
	{
		vel.y = -playerConfig.Speed;
		ip->up = false;
	}
	if (ip->down)
	{
		vel.y = +playerConfig.Speed;
		ip->down = false;
	}
	if (ip->right)
	{
		vel.x = +playerConfig.Speed;
		ip->right = false;
	}
	if (ip->left)
	{
		vel.x = -playerConfig.Speed;
		ip->left = false;
	}
	if (ip->shoot)
	{
		vec2 mousePos(sf::Mouse::getPosition(m_window).x, sf::Mouse::getPosition(m_window).y);
		spawnBullet(m_player, mousePos);
		ip->shoot = false;
	}

}

void Game::sLifeSpan()
{
	const EntityVec
		& bullets = m_eManager.getEntitiesWithTag("bullet"),
		& fragments = m_eManager.getEntitiesWithTag("fragment");

	for (int i = 0; i < bullets.size(); i++)
	{
		auto& e = bullets[i];

		int& rem = e->cLifeSpan->remaining;
		int& total = e->cLifeSpan->total;
		auto& circle = e->cShape->circle;

		//std::cout << rem << " " << total << std::endl;

		float newAlpha = 255 * ((float)rem) / total;

		sf::Color currFillColor = circle.getFillColor();
		currFillColor.a = newAlpha;
		circle.setFillColor(currFillColor);

		sf::Color currOutlineColor = circle.getOutlineColor();
		currOutlineColor.a = newAlpha;
		circle.setOutlineColor(currOutlineColor);

		if (rem == 0) e->destroy();
		else rem--;
	}

	for (int i = 0; i < fragments.size(); i++)
	{
		auto& e = fragments[i];

		int& rem = e->cLifeSpan->remaining;
		int& total = e->cLifeSpan->total;
		auto& circle = e->cShape->circle;

		std::cout << rem << " " << total << std::endl;

		float newAlpha = 255 * ((float)rem) / total;

		sf::Color currFillColor = circle.getFillColor();
		currFillColor.a = newAlpha;
		circle.setFillColor(currFillColor);

		sf::Color currOutlineColor = circle.getOutlineColor();
		currOutlineColor.a = newAlpha;
		circle.setOutlineColor(currOutlineColor);

		if (rem == 0) e->destroy();
		else rem--;
	}
}

void Game::sRender()
{
	for (auto& e : m_eManager.getEntities()) {

		m_window.draw(e->cShape->circle);
	}
}

void Game::sEnemySpawner()
{
	if (m_lastEnemySpawnTime != enemyConfig.SpawnInterval)
	{
		m_lastEnemySpawnTime++;
		return;
	}

	m_lastEnemySpawnTime = 0;
	spawnEnemy();
}

void Game::sCollision()
{
	float pRadius = m_player->cCollision->collisionRadius;

	for (auto& enemy : m_eManager.getEntitiesWithTag("enemy"))
	{

		float eRadius = enemy->cCollision->collisionRadius;
		if (m_player->cTransform->pos.distSq(enemy->cTransform->pos) < pRadius * pRadius + eRadius * eRadius)
		{
			enemy->destroy();
			m_player->destroy();
			spawnSmallEnemies(enemy);
			spawnPlayer();
		}

		for (auto& bullet : m_eManager.getEntitiesWithTag("bullet"))
		{
			float bRadius = bullet->cCollision->collisionRadius;
			if (bullet->cTransform->pos.distSq(enemy->cTransform->pos) < bRadius * bRadius + eRadius * eRadius)
			{
				bullet->destroy();
				enemy->destroy();
				spawnSmallEnemies(enemy);
			}
		}
	}
}

void Game::spawnPlayer()
{

	m_player = m_eManager.addEntity("player");

	float speed = playerConfig.Speed;

	float angle = randomInt(0, 360);
	float radians = (angle * PI) / 180;
	vec2 velocity(speed * cosf(radians), speed * sinf(radians));

	vec2 centerPos(m_window.getSize().x / 2, m_window.getSize().y / 2);

	m_player->cTransform = std::make_shared<CTransform>(centerPos, velocity, 0);

	m_player->cShape = std::make_shared<CShape>
		(
			playerConfig.ShapeRadius,
			playerConfig.ShapeVertices,
			playerConfig.FillColor,
			playerConfig.OutlineColor,
			playerConfig.OutlineThickness
		);

	m_player->cCollision = std::make_shared<CCollision>(playerConfig.CollisionRadius);
	m_player->cInput = std::make_shared<CInput>();

	//std::cout << playerConfig.ShapeRadius << " " << playerConfig.ShapeVertices << " " << playerConfig.OutlineThickness;


}

void Game::spawnEnemy()
{
	std::shared_ptr<Entity>e = m_eManager.addEntity("enemy");

	float speed = randomFloat(enemyConfig.SpeedMin, enemyConfig.SpeedMax);

	int radius = enemyConfig.ShapeRadius;

	float angle = randomInt(0, 360);
	float radians = (angle * PI) / 180;
	vec2 velocity(speed * cosf(radians), speed * sinf(radians));

	vec2 randomPos(randomFloat(radius, m_window.getSize().x - radius), randomFloat(radius, m_window.getSize().y - radius));

	e->cTransform = std::make_shared<CTransform>(randomPos, velocity, 0);


	int vertices = randomInt(enemyConfig.ShapeVerticesMin, enemyConfig.ShapeVerticesMax);

	int R = randomInt(100, 255), G = randomInt(100, 255), B = randomInt(100, 255);

	e->cShape = std::make_shared<CShape>
		(
			enemyConfig.ShapeRadius,
			vertices,
			sf::Color(R, G, B),
			enemyConfig.OutlineColor,
			enemyConfig.OutlineThickness
		);

	e->cCollision = std::make_shared<CCollision>(enemyConfig.CollisionRadius);
}

void Game::spawnSmallEnemies(std::shared_ptr<Entity> enemy)
{
	int pieces = enemy->cShape->circle.getPointCount(); // vertices

	const int vertices = pieces;

	float angleDifference = 360.0f / vertices;
	float angle = 0;

	auto& enemyCircle = enemy->cShape->circle;

	while (pieces--)
	{
		std::shared_ptr<Entity>fragment = m_eManager.addEntity("fragment");
		vec2 velocity(cos(angle), sin(angle));
		velocity *= 2;

		fragment->cTransform = std::make_shared<CTransform>(enemy->cTransform->pos, velocity, 0);

		fragment->cShape = std::make_shared<CShape>
			(
				enemyCircle.getRadius() / (2),
				vertices,
				enemyCircle.getFillColor(),
				enemyCircle.getOutlineColor(),
				enemyCircle.getOutlineThickness()
			);
		//std::cout << "SMLLL lifespan: " << enemyConfig.SmallPartLifespan << std::endl;
		fragment->cLifeSpan = std::make_shared<CLifeSpan>(enemyConfig.SmallPartLifespan);

		angle += angleDifference;
	}
}

void Game::spawnBullet(std::shared_ptr<Entity> srcEty, const vec2& destPos)
{
	std::shared_ptr<Entity>e = m_eManager.addEntity("bullet");

	vec2 velocity = destPos - srcEty->cTransform->pos;
	velocity.normalize();
	velocity *= bulletConfig.Speed;

	e->cTransform = std::make_shared<CTransform>(srcEty->cTransform->pos, velocity, 0);
	e->cShape = std::make_shared<CShape>
		(
			bulletConfig.ShapeRadius,
			bulletConfig.ShapeVertices,
			bulletConfig.FillColor,
			bulletConfig.OutlineColor,
			bulletConfig.OutlineThickness
		);
	e->cCollision = std::make_shared<CCollision>(bulletConfig.CollisionRadius);
	e->cLifeSpan = std::make_shared<CLifeSpan>(bulletConfig.Lifespan);
}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity)
{
}
