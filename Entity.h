#pragma once
#include <string>
#include <memory>
#include "Components.h"

class Game;
class EntityManager;

class Entity
{
	friend class EntityManager;
	friend class Game;

	size_t m_id;
	std::string m_tag = "default"; // player | enemy | bullet
	bool m_alive;

	Entity(size_t id, const std::string& tag) : m_id(id), m_tag(tag), m_alive(true) {};

public:

	std::shared_ptr  <CShape>       cShape;
	std::shared_ptr  <CTransform>   cTransform;
	std::shared_ptr  <CCollision>   cCollision;
	std::shared_ptr  <CScore>		cScore;
	std::shared_ptr  <CLifeSpan>    cLifeSpan;
	std::shared_ptr  <CInput>	    cInput;

	const size_t getId() const;
	const std::string& getTag() const;
	bool isAlive() const;
	void destroy();
};