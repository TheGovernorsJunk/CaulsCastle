#ifndef TE_RESOURCE_HOLDER_H
#define TE_RESOURCE_HOLDER_H

#include "types.h"

namespace te {

template <typename Resource>
class Resource_id {
	template <typename T = Resource>
	friend class Resource_holder;
	Resource_id(int id)
		: m_id{ id }
	{}
	int m_id;
public:
	Resource_id()
		: m_id{ 0 }
	{}
	inline bool operator==(const Resource_id& rhs) const
	{
		return m_id == rhs.m_id;
	}
	inline bool operator!=(const Resource_id& rhs) const
	{
		return !(*this == rhs);
	}
};

template <typename Resource>
class Resource_holder {
public:
	using Id = Resource_id<Resource>;

	template <typename T = Resource>
	Id insert(T&& resource)
	{
		m_resource_map.insert(std::pair<int, Resource>{ next_id, std::forward<T>(resource) });
		return{ next_id++ };
	}

	Resource& get(Id id)
	{
		auto found = m_resource_map.find(id.m_id);
		assert(found != m_resource_map.end());
		return found->second;
	}

	Resource_holder()
		: next_id{ 1 }
		, m_resource_map{}
	{}
private:
	int next_id;
	flat_map<int, Resource> m_resource_map;
};

} // namespace te

#endif
