#include "vehicle.h"

namespace te
{
	Vehicle::Vehicle(Game& world)
		: MovingEntity(world)
		, mSteering(*this)
	{}

	const SteeringBehaviors& Vehicle::getSteering() const
	{
		return mSteering;
	}

	SteeringBehaviors& Vehicle::getSteering()
	{
		return mSteering;
	}
}
