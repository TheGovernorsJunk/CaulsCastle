#ifndef TE_BASE_GAME_ENTITY_H
#define TE_BASE_GAME_ENTITY_H

namespace sf
{
	class Time;
}

namespace te
{
	struct Telegram;
	class EntityManager;

	class BaseGameEntity
	{
	public:
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