#include "base_game_entity.h"
#include "game.h"
#include "entity_manager.h"
#include "shape.h"

#include <cassert>

namespace te
{
	BaseGameEntity::BaseGameEntity(Game& world, sf::Vector2f position)
		: BaseGameEntity(world)
	{
		setPosition(position);
		world.getEntityManager().registerEntity(*this);
	}

	BaseGameEntity::BaseGameEntity(Game& world, const b2BodyDef& bodyDef)
		: BaseGameEntity(world)
	{
		world.getEntityManager().registerEntity(*this);

		mpBody = std::unique_ptr<b2Body, BodyDeleter>{mWorld.getPhysicsWorld().CreateBody(&bodyDef), [this](b2Body* pBody) {
			mWorld.getPhysicsWorld().DestroyBody(pBody);
		}};
		if (!mpBody) throw std::runtime_error("Unable to create b2Body in SceneNode.");
		mpBody->SetUserData(this);
	}

	BaseGameEntity::BaseGameEntity(Game& world)
		: mID(UNREGISTERED_ID)
		, mZ(0)
		, mMarkedForRemoval(false)
		, mBoundingRadius(1.f)
		, mpBody(nullptr)
		, mWorld(world)
	{}

	BaseGameEntity::~BaseGameEntity()
	{
		mWorld.getEntityManager().removeEntity(*this);
	}

	void BaseGameEntity::update(const sf::Time& dt)
	{
		if (mpBody)
		{
			b2Vec2 pos = mpBody->GetPosition();
			sf::Transformable::setPosition(pos.x, pos.y);
		}
		onUpdate(dt);
	}

	void BaseGameEntity::setBoundingRadius(float radius)
	{
		mBoundingRadius = radius;
	}

	float BaseGameEntity::getBoundingRadius() const
	{
		return mBoundingRadius;
	}

	bool BaseGameEntity::handleMessage(const Telegram& msg)
	{
		return false;
	}

	EntityID BaseGameEntity::getID() const
	{
		return mID;
	}

	const Game& BaseGameEntity::getWorld() const
	{
		return mWorld;
	}

	Game& BaseGameEntity::getWorld()
	{
		return mWorld;
	}

	//void BaseGameEntity::attachNodeByID(EntityID child)
	//{
	//	BaseGameEntity& entity = getWorld().getEntityManager().getEntityFromID(child);
	//	std::unique_ptr<SceneNode> pEntity = entity.getParent().detachNode(entity);
	//	SceneNode::attachNode(std::move(pEntity));
	//}

	void BaseGameEntity::setDrawOrder(int z)
	{
		mZ = z;
	}

	int BaseGameEntity::getDrawOrder() const
	{
		return mZ;
	}

	const sf::Transform& BaseGameEntity::getTransform() const
	{
		return sf::Transformable::getTransform();
	}

	const sf::Vector2f& BaseGameEntity::getPosition() const
	{
		return sf::Transformable::getPosition();
	}

	void BaseGameEntity::attachRigidBody(int bodyType)
	{
		assert(bodyType >= 0 && bodyType <= 2);
		if (!mpBody)
		{
			b2BodyDef bodyDef;
			bodyDef.type = (b2BodyType)bodyType;
			sf::Vector2f worldPosition = getTransform().transformPoint({ 0, 0 });
			bodyDef.position = { worldPosition.x, worldPosition.y };
			mpBody = { mWorld.getPhysicsWorld().CreateBody(&bodyDef), [this](b2Body* pBody) { mWorld.getPhysicsWorld().DestroyBody(pBody); } };
			mpBody->SetUserData(this);
		}
		else
		{
			throw std::runtime_error("Node already has rigid body.");
		}
	}

	void BaseGameEntity::attachFixture(const Shape* pShape) const
	{
		assert(pShape);
		if (!mpBody) throw std::runtime_error{"Attaching a fixture requires a rigid body."};
		mpBody->CreateFixture(&pShape->getShape(), 0);
	}

	void BaseGameEntity::setVelocity(sf::Vector2f vel)
	{
		if (!mpBody) throw std::runtime_error{"Setting velocity requires a rigid body."};
		mpBody->SetLinearVelocity(b2Vec2{vel.x, vel.y});
	}

	b2Body& BaseGameEntity::getBody()
	{
		if (!mpBody) throw std::runtime_error{"Rigid body not set."};
		return *mpBody;
	}
}
