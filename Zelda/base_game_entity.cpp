#include "base_game_entity.h"

namespace te
{
	BaseGameEntity::BaseGameEntity(Game& pWorld)
		: sf::Transformable()
		, sf::Drawable()
		, mID(UNREGISTERED_ID)
		, mBoundingRadius(1.f)
		, mWorld(pWorld)
	{}

	BaseGameEntity::~BaseGameEntity() {}

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

	void BaseGameEntity::draw(sf::RenderTarget&, sf::RenderStates) const {}
}
