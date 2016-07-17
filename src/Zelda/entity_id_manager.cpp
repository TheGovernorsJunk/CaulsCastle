#include "entity_id_manager.h"

namespace te
{
	EntityIDManager::EntityIDManager()
		: m_NextID{ 1 }
	{}

	EntityID EntityIDManager::getNextID()
	{
		return m_NextID++;
	}
}
