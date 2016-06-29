#include "rigid_body.h"
#include "base_game_entity.h"
#include "game.h"
#include "utilities.h"
#include "shape.h"

namespace te
{
	std::unique_ptr<RigidBody> RigidBody::make(BaseGameEntity& owner, b2BodyType bodyType)
	{
		return std::unique_ptr<RigidBody>{new RigidBody{owner, bodyType}};
	}

	RigidBody::RigidBody(BaseGameEntity& owner, b2BodyType bodyType)
		: m_Owner{owner}
		, m_World{owner.getWorld()}
		, m_PhysicsWorld{m_World.getPhysicsWorld()}
		, m_pBody{}
	{
		b2BodyDef bodyDef;
		bodyDef.type = bodyType;
		sf::Vector2f position = m_Owner.getPosition();
		bodyDef.position = b2Vec2{position.x, position.y};
		m_pBody = std::unique_ptr<b2Body, BodyDeleter>{m_PhysicsWorld.CreateBody(&bodyDef), [this](b2Body* pBody) { m_PhysicsWorld.DestroyBody(pBody); }};

		if (!m_pBody) throw std::runtime_error{"RigidBody ctor: Could not create rigid body."};
		if (m_Owner.hasComponent<RigidBody>()) throw std::runtime_error{"RigidBody ctor: Entity already has rigid body."};

		m_pBody->SetUserData(&m_Owner);
	}

	sf::Vector2f RigidBody::getPosition() const
	{
		b2Vec2 position = m_pBody->GetPosition();
		return sf::Vector2f{position.x, position.y};
	}

	float RigidBody::getRotation() const
	{
		return m_pBody->GetAngle() * (180.f / PI);
	}

	void RigidBody::setVelocity(sf::Vector2f velocity)
	{
		m_pBody->SetLinearVelocity(b2Vec2{velocity.x, velocity.y});
	}

	void RigidBody::attachFixture(const Shape* pShape) const
	{
		assert(pShape);
		m_pBody->CreateFixture(&pShape->getShape(), 0);
	}

	void RigidBody::update(const sf::Time& dt)
	{
		m_Owner.setPosition(getPosition());
	}
}
