#include "EntityManager.h"

void EntityManager::removeDeadEntities(EntityVec& vec)
{
	auto new_end=std::remove_if(vec.begin(), vec.end(), [](std::shared_ptr<Entity> e) {return !e->isActive(); });
	vec.erase(new_end,vec.end());
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
{
	std::shared_ptr<Entity> e(new Entity(m_totalEntities++,tag));
	m_entitiesToAdd.push_back(e);
	return e;
}

void EntityManager::update()
{
	for (auto e : m_entitiesToAdd)
	{
		m_entities.push_back(e);
		m_entityMap[e->m_tag].push_back(e);
	}
	m_entitiesToAdd.clear();

	removeDeadEntities(m_entities);

	for (auto& vec : m_entityMap)
	{
		removeDeadEntities(vec.second);
	}
}

EntityVec& EntityManager::getEntities()
{
	return m_entities;
}


EntityVec& EntityManager::getEntities(const std::string& tag)
{
	return m_entityMap[tag];
}


const std::map<std::string, EntityVec>& EntityManager::getEntityMap()
{
	return m_entityMap;
}
