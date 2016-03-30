#include "base_game_entity.h"

namespace te
{
	BaseGameEntity::BaseGameEntity(const std::shared_ptr<Game>& pWorld)
		: sf::Transformable()
		, sf::Drawable()
		, mID(UNREGISTERED_ID)
		, mBoundingRadius(1.f)
		, mpWorld(pWorld)
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
		return *mpWorld;
	}

	Game& BaseGameEntity::getWorld()
	{
		return *mpWorld;
	}

	void BaseGameEntity::draw(sf::RenderTarget&, sf::RenderStates) const {}
}
