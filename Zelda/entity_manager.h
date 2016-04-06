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
		static std::unique_ptr<EntityManager> make();

		void registerEntity(BaseGameEntity&);
		BaseGameEntity& getEntityFromID(int id) const;
		void removeEntity(BaseGameEntity&);
	private:
		EntityManager();

		EntityManager(const EntityManager&) = delete;
		EntityManager& operator=(const EntityManager&) = delete;

		std::map<int, BaseGameEntity*> mEntityMap;
		int mEntityCount;
	};
}

#endif
