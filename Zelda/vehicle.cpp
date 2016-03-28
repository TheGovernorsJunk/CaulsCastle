#include "vehicle.h"

namespace te
{
	Vehicle::Vehicle(const std::shared_ptr<Game>& pWorld)
		: MovingEntity(pWorld)
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
