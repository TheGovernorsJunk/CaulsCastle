#ifndef TE_VEHICLE_H
#define TE_VEHICLE_H

#include "moving_entity.h"
#include "steering_behaviors.h"

namespace te
{
	class Vehicle : public MovingEntity
	{
	public:
		Vehicle(Game& pWorld);

		const SteeringBehaviors& getSteering() const;
		SteeringBehaviors& getSteering();

	private:
		SteeringBehaviors mSteering;
	};
}

#endif
