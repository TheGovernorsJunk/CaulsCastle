#include "entity_manager.h"
#include "base_game_entity.h"
#include <algorithm>

namespace te
{
	std::unique_ptr<EntityManager> EntityManager::make()
	{
		return std::unique_ptr<EntityManager>(new EntityManager());
	}

	EntityManager::EntityManager()
		: mEntityMap()
		, mEntityCount(0)
	{}

	void EntityManager::registerEntity(std::shared_ptr<BaseGameEntity> entity)
	{
		if (entity->mID == 0)
		{
			mEntityMap.insert(std::make_pair(++mEntityCount, entity));
			entity->mID = mEntityCount;
		}
		else
		{
			throw std::runtime_error("Entity already registered.");
		}
	}

	std::shared_ptr<BaseGameEntity> EntityManager::getEntityFromID(int id) const
	{
		auto iter = mEntityMap.find(id);
		if (iter != mEntityMap.end())
		{
			return iter->second;
		}
		else
		{
			throw std::runtime_error("Entity does not exist.");
		}
	}

	void EntityManager::removeEntity(std::shared_ptr<BaseGameEntity> entity)
	{
		auto iter = mEntityMap.find(entity->mID);
		if (iter != mEntityMap.end())
		{
			mEntityMap.erase(iter);
			entity->mID = 0;
		}
	}
}
