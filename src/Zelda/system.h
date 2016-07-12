#ifndef TE_SYSTEM_H
#define TE_SYSTEM_H

#include <SFML/Graphics.hpp>

#include <vector>

namespace te
{
	class System
	{
	public:
		System() {}
		virtual ~System() {}
		constexpr System(System&&) noexcept = default;
		System& operator=(System&&) noexcept = default;

		virtual void update(const sf::Time& dt) = 0;
	};

	template <typename T>
	class SystemImpl : public System
	{
	public:
		SystemImpl();

		SystemImpl(SystemImpl&&) noexcept = default;
		SystemImpl& operator=(SystemImpl&&) noexcept = default;

		SystemImpl(const SystemImpl&) = delete;
		SystemImpl& operator=(const SystemImpl&) = delete;

		void update(const sf::Time& dt) override;

		template <typename... Args>
		void addComponent(Args&&... args);
	private:
		std::vector<T> m_Components;
	};
}

#include "system.inl"
#endif
