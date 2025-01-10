#pragma once
#include <vector>
#include <memory>
#include <unordered_map>
#include "Entity.h"


typedef std::vector<std::shared_ptr<Entity>> EntityVec;
typedef std::unordered_map<std::string, EntityVec> EntityMap;

class Game;
class EntityManager
{
	friend class Game;

	EntityVec m_entities;
	EntityMap m_entityMap;
	size_t m_tillNowCount = 0;
	EntityVec m_entitiesToAdd;

	EntityManager(){}

public:

	std::shared_ptr<Entity> addEntity(const std::string &tag);
	const EntityVec& getEntities() const;
	const EntityVec& getEntitiesWithTag(const std::string &tag);
	void update();
};