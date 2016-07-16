#ifndef TE_COMPONENT_STORE_H
#define TE_COMPONENT_STORE_H

#include <boost/container/flat_map.hpp>

namespace te
{
	template <typename Component>
	class ComponentStore
	{
	public:
		decltype(auto) begin() { return m_Components.begin(); }
		decltype(auto) begin() const { return m_Components.begin(); }
		decltype(auto) end() { return m_Components.end(); }
		decltype(auto) end() const { return m_Components.end(); }
		decltype(auto) cbegin() const { return m_Components.cbegin(); }
		decltype(auto) cend() const { return m_Components.cend(); }

		inline Component& operator[](int index)
		{
			return m_Components[index];
		}

		inline Component& at(int index)
		{
			return m_Components.at(index);
		}

		inline bool contains(int index) const
		{
			return m_Components.find(index) != m_Components.end();
		}
	private:
		boost::container::flat_map<int, Component> m_Components;
	};
}

#endif
