#include "base_game_entity.h"
#include "game.h"
#include "entity_manager.h"

namespace te
{
	BaseGameEntity::BaseGameEntity(Game& world, sf::Vector2f position)
		: SceneNode(world, position)
		, mID(UNREGISTERED_ID)
		, mBoundingRadius(1.f)
		, mWorld(world)
	{
		world.getEntityManager().registerEntity(*this);
	}

	BaseGameEntity::BaseGameEntity(Game& world, const b2BodyDef& bodyDef)
		: SceneNode(world, bodyDef)
		, mID(UNREGISTERED_ID)
		, mBoundingRadius(1.f)
		, mWorld(world)
	{
		world.getEntityManager().registerEntity(*this);
	}

	BaseGameEntity::~BaseGameEntity()
	{
		mWorld.getEntityManager().removeEntity(*this);
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

	int BaseGameEntity::getID() const
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
}
