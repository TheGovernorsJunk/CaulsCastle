#ifndef TE_MANAGER_RUNNER_H
#define TE_MANAGER_RUNNER_H

#include <memory>

namespace sf
{
	class RenderTarget;
	class Time;
}

namespace te
{
	struct GameData;

	class ManagerRunner
	{
	public:
		ManagerRunner(GameData& data);
		ManagerRunner(ManagerRunner&&) noexcept;
		ManagerRunner& operator=(ManagerRunner&&) noexcept;
		~ManagerRunner();

		void fixedUpdate(const sf::Time& dt);
		void renderUpdate();
	private:
		struct Impl;
		std::unique_ptr<Impl> m_pImpl;
	};
}

#endif
