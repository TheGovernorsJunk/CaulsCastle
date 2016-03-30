#ifndef TE_ZELDA_ENTITY_H
#define TE_ZELDA_ENTITY_H

#include "moving_entity.h"
#include "path_planner.h"
#include "goal_think.h"
#include "regulator.h"
#include "steering_behaviors.h"

namespace te
{
	class Game;

	class ZeldaEntity : public MovingEntity
	{
	public:
		ZeldaEntity(const std::shared_ptr<Game>& pGame);

		void update(const sf::Time& dt);
		PathPlanner& getPathPlanner();
		GoalThink& getBrain();
		SteeringBehaviors& getSteering();

	private:
		void draw(sf::RenderTarget&, sf::RenderStates) const;

		PathPlanner mPathPlanner;

		Regulator mGoalArbitrationRegulator;
		GoalThink mBrain;
		SteeringBehaviors mSteering;
	};
}

#endif
