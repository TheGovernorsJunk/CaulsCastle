#ifndef TE_COMPONENT_STORE_H
#define TE_COMPONENT_STORE_H

#include "typedefs.h"

#include <boost/container/flat_map.hpp>

#include <vector>

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

		inline Component& operator[](EntityID index)
		{
			return m_Components[index];
		}

		inline Component& at(EntityID index)
		{
			return m_Components.at(index);
		}

		inline bool contains(EntityID index) const
		{
			return m_Components.find(index) != m_Components.end();
		}
	private:
		boost::container::flat_map<EntityID, Component> m_Components;
	};

	template <typename Component>
	class MultiComponentStore
	{
	public:
		decltype(auto) begin() { return m_Components.begin(); }
		decltype(auto) begin() const { return m_Components.begin(); }
		decltype(auto) end() { return m_Components.end(); }
		decltype(auto) end() const { return m_Components.end(); }
		decltype(auto) cbegin() const { return m_Components.cbegin(); }
		decltype(auto) cend() const { return m_Components.cend(); }

		template <typename Iter>
		void at(EntityID index, Iter out)
		{
			for (auto& componentPair : m_Components)
			{
				if (componentPair.first == index)
				{
					*out++ = &componentPair.second;
				}
			}
		}

		template <typename T = Component>
		void insert(EntityID index, T&& component)
		{
			m_Components.push_back({ index, std::forward<T>(component) });
		}

	private:
		std::vector<std::pair<EntityID, Component>> m_Components;
	};

	template <typename Drawable>
	struct Renderable
	{
		Drawable drawable;
		int sortingLayer;
	};
	template <typename Drawable>
	using RenderableStore = MultiComponentStore<Renderable<Drawable>>;
}

#endif
