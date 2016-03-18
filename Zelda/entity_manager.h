#ifndef TE_ENTITY_MANAGER_H
#define TE_ENTITY_MANAGER_H

#include <map>
#include <memory>

namespace te
{
	class BaseGameEntity;

	class EntityManager
	{
	public:
		EntityManager();

		void registerEntity(std::shared_ptr<BaseGameEntity>);
		std::shared_ptr<BaseGameEntity> getEntityFromID(int id) const;
		void removeEntity(std::shared_ptr<BaseGameEntity>);
	private:
		EntityManager(const EntityManager&) = delete;
		EntityManager& operator=(const EntityManager&) = delete;

		std::map<int, std::shared_ptr<BaseGameEntity>> mEntityMap;
		int mEntityCount;
	};
}

#endif
