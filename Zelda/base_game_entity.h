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

	class BaseGameEntity : public sf::Transformable, public sf::Drawable
	{
	public:
		const static int UNREGISTERED_ID = 0;

		BaseGameEntity(Game& pWorld);
		virtual ~BaseGameEntity();

		void setBoundingRadius(float radius);
		float getBoundingRadius() const;
		virtual void update(const sf::Time& dt) = 0;
		virtual bool handleMessage(const Telegram& msg);
		int getID() const;
		const Game& getWorld() const;
		Game& getWorld();
	private:
		virtual void draw(sf::RenderTarget&, sf::RenderStates) const;
		friend class EntityManager;

		int mID;
		float mBoundingRadius;
		Game& mWorld;
	};
}

#endif
