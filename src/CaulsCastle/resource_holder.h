#ifndef TE_RESOURCE_HOLDER_H
#define TE_RESOURCE_HOLDER_H

#include "types.h"

namespace te
{
	template <typename Resource>
	class Resource_id {
	public:
		Resource_id(int id)
			: m_id{ id }
		{}
	private:
		template<typename T = Resource>
		friend class Resource_holder;
		int m_id;
	};

	template <typename Resource>
	class Resource_holder {
	public:
		using Id = Resource_id<Resource>;

		template <typename T = Resource>
		Id insert(T&& resource)
		{
			m_resource_map.insert(std::pair<int, Resource>{ next_id, std::forward<T>(resource) });
			return { next_id++ };
		}

		Resource& get(Id id)
		{
			auto found = m_resource_map.find(id.m_id);
			assert(found != m_resource_map.end());
			return found->second;
		}
	private:
		int next_id;
		flat_map<int, Resource> m_resource_map;
	};
}

#endif
