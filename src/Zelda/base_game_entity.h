#ifndef TE_BASE_GAME_ENTITY_H
#define TE_BASE_GAME_ENTITY_H

#include "scene_node.h"

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

#include <memory>
#include <functional>

namespace sf
{
	class Time;
}

namespace te
{
	struct Telegram;
	class EntityManager;
	class Game;

	class BaseGameEntity : public SceneNode
	{
	public:
		const static int UNREGISTERED_ID = 0;

		BaseGameEntity(Game& pWorld, sf::Vector2f position);
		BaseGameEntity(Game& pWorld, const b2BodyDef&);
		virtual ~BaseGameEntity();

		void setBoundingRadius(float radius);
		float getBoundingRadius() const;
		virtual bool handleMessage(const Telegram& msg);
		int getID() const;
		const Game& getWorld() const;
		Game& getWorld();

	private:
		friend class EntityManager;

		int mID;
		float mBoundingRadius;
		Game& mWorld;
	};
}

#endif
