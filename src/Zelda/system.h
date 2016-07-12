#ifndef TE_SYSTEM_H
#define TE_SYSTEM_H

#include <SFML/Graphics.hpp>

#include <vector>

namespace te
{
	class System
	{
	public:
		virtual ~System() {}
		System(System&&) = default;
		System& operator=(System&&) = default;

		virtual void update(const sf::Time& dt) = 0;
	};

	template <typename T>
	class SystemImpl : public System
	{
	public:
		SystemImpl();

		SystemImpl(SystemImpl&&) = default;
		SystemImpl& operator=(SystemImpl&&) = default;

		SystemImpl(const SystemImpl&) = delete;
		SystemImpl& operator=(const SystemImpl&) = delete;

		void update(const sf::Time& dt) override;
	private:
		std::vector<T> m_Components;
	};
}

#include "system.inl"
#endif
