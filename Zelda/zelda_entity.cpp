#include "zelda_entity.h"

namespace te
{
	ZeldaEntity::ZeldaEntity(const std::shared_ptr<Game>& pGame)
		: MovingEntity(pGame)
		, mPathPlanner(*this)
		, mGoalArbitrationRegulator(sf::seconds(0.5f))
		, mBrain(*this)
	{}

	void ZeldaEntity::update(const sf::Time& dt)
	{
		mBrain.process(dt);

		if (mGoalArbitrationRegulator.isReady(dt)) mBrain.arbitrate();
	}

	PathPlanner& ZeldaEntity::getPathPlanner()
	{
		return mPathPlanner;
	}

	void ZeldaEntity::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		target.draw(sf::CircleShape(getBoundingRadius()), states);
	}
}
