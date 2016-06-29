#ifndef TE_RIGID_BODY_H
#define TE_RIGID_BODY_H

#include "component.h"

#include <SFML/System/Vector2.hpp>
#include <Box2D/Box2D.h>

#include <memory>
#include <functional>

class b2World;
class b2Body;

namespace te
{
	class BaseGameEntity;
	class Game;
	class Shape;

	class RigidBody : public Component
	{
	public:
		static std::unique_ptr<RigidBody> make(BaseGameEntity& owner, b2BodyType bodyType);

		sf::Vector2f getPosition() const;
		float getRotation() const;
		void setVelocity(sf::Vector2f velocity);
		void attachFixture(const Shape* pShape) const;

	private:
		using BodyDeleter = std::function<void(b2Body*)>;

		RigidBody(BaseGameEntity& owner, b2BodyType bodyType);

		void update(const sf::Time& dt);

		BaseGameEntity& m_Owner;
		Game& m_World;
		b2World& m_PhysicsWorld;
		std::unique_ptr<b2Body, BodyDeleter> m_pBody;
	};
}

#endif
