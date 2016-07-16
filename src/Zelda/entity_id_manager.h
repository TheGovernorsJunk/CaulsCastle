#ifndef TE_ENTITY_ID_MANAGER_H
#define TE_ENTITY_ID_MANAGER_H

#include "typedefs.h"

namespace te
{
	class EntityIDManager
	{
	public:
		EntityIDManager();
		EntityIDManager(EntityIDManager&&) = default;
		EntityIDManager& operator=(EntityIDManager&&) = default;

		EntityID getNextID();
	private:
		EntityID m_NextID;
	};
}

#endif
