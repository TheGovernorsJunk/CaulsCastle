#ifndef TE_RESOURCE_MANAGER_H
#define TE_RESOURCE_MANAGER_H

#include <map>
#include <memory>
#include <cassert>

namespace te
{
	template <typename Resource>
	struct ResourceID
	{
		int value;
	};

	template <typename Resource>
	class ResourceManager
	{
	public:
		ResourceManager();

		template <typename... Args>
		ResourceID<Resource> load(const std::string& filename, Args&&... args);

		Resource& get(ResourceID<Resource> id);
		const Resource& get(ResourceID<Resource> id) const;

	private:
		ResourceID<Resource> insertResource(std::unique_ptr<Resource>&& pResource);

		int m_NextID;
		std::map<int, std::unique_ptr<Resource>> m_ResourceMap;
	};
}

#include "resource_manager.inl"
#endif
