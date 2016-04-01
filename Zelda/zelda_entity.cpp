#include "zelda_entity.h"

namespace te
{
	ZeldaEntity::ZeldaEntity(Game& game)
		: MovingEntity(game)
		, mPathPlanner(*this)
		, mGoalArbitrationRegulator(sf::seconds(0.5f))
		, mBrain(*this)
		, mSteering(*this)
	{}

	void ZeldaEntity::update(const sf::Time& dt)
	{
		mBrain.process(dt);
		updateOnForce(dt, mSteering.calculate());

		if (mGoalArbitrationRegulator.isReady(dt)) mBrain.arbitrate();
	}

	PathPlanner& ZeldaEntity::getPathPlanner()
	{
		return mPathPlanner;
	}

	GoalThink& ZeldaEntity::getBrain()
	{
		return mBrain;
	}

	SteeringBehaviors& ZeldaEntity::getSteering()
	{
		return mSteering;
	}

	void ZeldaEntity::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		target.draw(sf::CircleShape(getBoundingRadius()), states);
	}
}
