#ifndef TE_BASE_GAME_ENTITY_H
#define TE_BASE_GAME_ENTITY_H

#include <SFML/Graphics.hpp>

namespace sf
{
	class Time;
}

namespace te
{
	struct Telegram;
	class EntityManager;

	class BaseGameEntity : public sf::Transformable
	{
	public:
		const static int UNREGISTERED_ID = 0;

		BaseGameEntity();
		virtual void update(const sf::Time& dt) = 0;
		virtual bool handleMessage(const Telegram& msg) = 0;
		int getID() const;
	private:
		friend class EntityManager;
		int mID;
	};
}

#endif
