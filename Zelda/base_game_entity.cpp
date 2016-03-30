#include "base_game_entity.h"

namespace te
{
	BaseGameEntity::BaseGameEntity(const std::shared_ptr<Game>& pWorld)
		: sf::Transformable()
		, sf::Drawable()
		, mID(UNREGISTERED_ID)
		, mpWorld(pWorld)
	{}

	BaseGameEntity::~BaseGameEntity() {}

	float BaseGameEntity::getBoundingRadius() const
	{
		return 1.f;
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
