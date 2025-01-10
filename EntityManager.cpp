#include "EntityManager.h"
#include <algorithm>

std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
{
    std::shared_ptr<Entity> e(new Entity(m_tillNowCount, tag));
    
    m_entities.push_back(e);
    m_entityMap[tag].push_back(e);
    
    return e;
}

const EntityVec& EntityManager::getEntities() const
{
    return m_entities;
}

const EntityVec& EntityManager::getEntitiesWithTag(const std::string& tag) 
{   
    return m_entityMap[tag];
}

void EntityManager::update()
{
    m_entities.erase(
        remove_if(m_entities.begin(), m_entities.end(), [](std::shared_ptr<Entity> e)
            {
                return !e->isAlive();
            }
        ),
        m_entities.end()
    );

    for (auto& itr : m_entityMap)
    {
        itr.second.erase(
            remove_if(itr.second.begin(), itr.second.end(), [](std::shared_ptr<Entity> e)
                {
                    return !e->isAlive();
                }
            ),
            itr.second.end()
        );
    }

    for (auto& e : m_entitiesToAdd)
    {
        m_entities.push_back(e);
        m_entityMap[e->m_tag].push_back(e);
    }
    m_entitiesToAdd.clear();
}

