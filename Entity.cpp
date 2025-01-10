#include "Entity.h"

const size_t Entity::getId() const
{
    return size_t();
}

const std::string& Entity::getTag() const
{
    return m_tag;
}

bool Entity::isAlive() const
{
    return m_alive;
}

void Entity::destroy()
{
    m_alive = false;
}
