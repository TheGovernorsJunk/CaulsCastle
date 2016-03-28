#ifndef TE_BASE_GAME_ENTITY_H
#define TE_BASE_GAME_ENTITY_H

#include <SFML/Graphics.hpp>

#include <memory>

namespace sf
{
	class Time;
}

namespace te
{
	struct Telegram;
	class EntityManager;
	class Game;

	class BaseGameEntity : public sf::Transformable
	{
	public:
		const static int UNREGISTERED_ID = 0;

		BaseGameEntity(const std::shared_ptr<Game>& pWorld);
		virtual ~BaseGameEntity();

		virtual void update(const sf::Time& dt) = 0;
		virtual bool handleMessage(const Telegram& msg) = 0;
		int getID() const;
		const Game& getWorld() const;
		Game& getWorld();
	private:
		friend class EntityManager;
		int mID;
		std::shared_ptr<Game> mpWorld;
	};
}

#endif
