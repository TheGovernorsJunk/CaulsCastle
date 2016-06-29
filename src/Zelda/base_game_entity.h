#ifndef TE_BASE_GAME_ENTITY_H
#define TE_BASE_GAME_ENTITY_H

#include "scene_node.h"
#include "typedefs.h"

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

#include <memory>
#include <functional>

namespace te
{
	struct Telegram;
	class EntityManager;
	class Game;
	class Shape;

	class BaseGameEntity : private sf::Transformable, public sf::Drawable
	{
	public:
		const static int UNREGISTERED_ID = 0;

		BaseGameEntity(Game& pWorld, sf::Vector2f position);
		BaseGameEntity(Game& pWorld, const b2BodyDef&);
		virtual ~BaseGameEntity();

		void update(const sf::Time& dt);
		void setBoundingRadius(float radius);
		float getBoundingRadius() const;
		virtual bool handleMessage(const Telegram& msg);
		EntityID getID() const;
		const Game& getWorld() const;
		Game& getWorld();
		//void attachNodeByID(EntityID child);
		void setDrawOrder(int z);
		int getDrawOrder() const;
		const sf::Transform& getTransform() const;
		void setPosition(const sf::Vector2f& position) { sf::Transformable::setPosition(position); }
		const sf::Vector2f& getPosition() const;
		void move(float x, float y) { sf::Transformable::move(x, y); }
		void die() { mMarkedForRemoval = true; }
		bool isMarkedForRemoval() const { return mMarkedForRemoval; }

		// use b2BodyType enum; Lua needs int to interface
		void attachRigidBody(int);
		void attachFixture(const Shape* pShape) const;
		void setVelocity(sf::Vector2f vel);

	protected:
		virtual void onUpdate(const sf::Time& dt) {}

	private:
		friend class EntityManager;

		BaseGameEntity(Game& World);

		void draw(sf::RenderTarget&, sf::RenderStates) const {}

		int mID;
		int mZ;
		bool mMarkedForRemoval;
		float mBoundingRadius;
		using BodyDeleter = std::function<void(b2Body*)>;
		std::unique_ptr<b2Body, BodyDeleter> mpBody;
		Game& mWorld;
	};
}

#endif
