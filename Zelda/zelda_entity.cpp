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
		mBrain.process();

		if (mGoalArbitrationRegulator.isReady(dt)) mBrain.arbitrate();
	}
}
