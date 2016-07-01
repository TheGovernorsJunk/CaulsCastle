#ifndef TE_RESOURCE_MANAGER_H
#define TE_RESOURCE_MANAGER_H

#include <map>
#include <memory>
#include <cassert>

namespace te
{
	template <typename Resource>
	class ResourceManager
	{
	public:
		template <typename... Args>
		void load(const std::string& id, const std::string& filename, Args&&... args);

		Resource& get(const std::string& id);
		const Resource& get(const std::string& id) const;

	private:
		void insertResource(const std::string& id, std::unique_ptr<Resource>&& pResource);
		std::map<std::string, std::unique_ptr<Resource>> m_ResourceMap;
	};
}

#include "resource_manager.inl"
#endif
