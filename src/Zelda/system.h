#ifndef TE_SYSTEM_H
#define TE_SYSTEM_H

#include <SFML/Graphics.hpp>

#include <vector>

namespace te
{
	namespace detail
	{
		template <typename T>
		class SystemStorage
		{
		public:
			SystemStorage() noexcept = default;
			SystemStorage(SystemStorage&&) noexcept = default;
			SystemStorage& operator=(SystemStorage&&) noexcept = default;

			auto begin() noexcept
			{
				return m_Components.begin();
			}
			auto end() noexcept
			{
				return m_Components.end();
			}
			template <typename U = T>
			void add(U&& component)
			{
				m_Components.push_back(std::forward<U>(component));
			}

		private:
			std::vector<T> m_Components;
		};
	}

	class System
	{
	public:
		System() noexcept = default;
		virtual ~System() = default;
		constexpr System(System&&) noexcept = default;
		System& operator=(System&&) noexcept = default;

		virtual void update(const sf::Time& dt) = 0;
	};

	template <typename T>
	class SystemBase
	{
	public:
		SystemBase() noexcept = default;
		virtual ~SystemBase() = default;
		SystemBase(SystemBase&&) noexcept = default;
		SystemBase& operator=(SystemBase&&) noexcept = default;

		template <typename... Args>
		void addComponent(Args&&... args)
		{
			m_Components.add(T(std::forward<Args>(args)...));
		}

	protected:
		detail::SystemStorage<T> m_Components;
	};

	template <typename T>
	class SystemImpl : public System, public SystemBase<T>
	{
	public:
		SystemImpl() noexcept = default;
		SystemImpl(SystemImpl&&) noexcept = default;
		SystemImpl& operator=(SystemImpl&&) noexcept = default;

		void update(const sf::Time& dt) override;
	};
}

#include "system.inl"
#endif
