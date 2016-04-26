#ifndef TE_GAME_STATE_H
#define TE_GAME_STATE_H

#include <SFML/Graphics.hpp>

namespace te
{
	class StateStack;

	class GameState : public sf::Drawable
	{
	public:
		virtual ~GameState() {}

		virtual bool processInput(const sf::Event& evt) = 0;
		virtual bool update(const sf::Time& dt) = 0;
	protected:
		template<typename T, typename... Args>
		void pushState(Args&&... args)
		{
			mStateStack->queuePush(T::make(std::forward<Args>(args)...));
		}
		void popState();
		void clearStates();

	private:
		friend class StateStack;

		StateStack* mStateStack;
	};
}

#endif
