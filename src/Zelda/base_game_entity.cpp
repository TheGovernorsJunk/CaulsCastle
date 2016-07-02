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
	}

	BaseGameEntity::BaseGameEntity(Game& world)
		: mID(UNREGISTERED_ID)
		, mMarkedForRemoval(false)
		, mComponents()
		, mWorld(world)
	{}

	BaseGameEntity::~BaseGameEntity()
	{
		if (mID != UNREGISTERED_ID) mWorld.getEntityManager().removeEntity(*this);
	}

	void BaseGameEntity::update(const sf::Time& dt)
	{
		for (auto& component : mUpdateComponents) component->update(dt);
		onUpdate(dt);
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

	const sf::Transform& BaseGameEntity::getTransform() const
	{
		return sf::Transformable::getTransform();
	}

	const sf::Vector2f& BaseGameEntity::getPosition() const
	{
		return sf::Transformable::getPosition();
	}

	//void BaseGameEntity::attachRigidBody(int bodyType)
	//{
	//	assert(bodyType >= 0 && bodyType <= 2);
	//	if (!mpBody)
	//	{
	//		b2BodyDef bodyDef;
	//		bodyDef.type = (b2BodyType)bodyType;
	//		sf::Vector2f worldPosition = getTransform().transformPoint({ 0, 0 });
	//		bodyDef.position = { worldPosition.x, worldPosition.y };
	//		mpBody = { mWorld.getPhysicsWorld().CreateBody(&bodyDef), [this](b2Body* pBody) { mWorld.getPhysicsWorld().DestroyBody(pBody); } };
	//		mpBody->SetUserData(this);
	//	}
	//	else
	//	{
	//		throw std::runtime_error("Node already has rigid body.");
	//	}
	//}

	//void BaseGameEntity::attachFixture(const Shape* pShape) const
	//{
	//	assert(pShape);
	//	if (!mpBody) throw std::runtime_error{"Attaching a fixture requires a rigid body."};
	//	mpBody->CreateFixture(&pShape->getShape(), 0);
	//}

	//void BaseGameEntity::setVelocity(sf::Vector2f vel)
	//{
	//	if (!mpBody) throw std::runtime_error{"Setting velocity requires a rigid body."};
	//	mpBody->SetLinearVelocity(b2Vec2{vel.x, vel.y});
	//}

	//b2Body& BaseGameEntity::getBody()
	//{
	//	if (!mpBody) throw std::runtime_error{"Rigid body not set."};
	//	return *mpBody;
	//}
}
