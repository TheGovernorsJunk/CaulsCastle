#include "goal_move_to_position.h"
#include "zelda_entity.h"
#include "goal_follow_path.h"

namespace te
{
	Goal_MoveToPosition::Goal_MoveToPosition(ZeldaEntity& owner, sf::Vector2f position)
		: mOwner(owner)
		, mPosition(position)
	{}

	void Goal_MoveToPosition::activate()
	{
		setStatus(Status::ACTIVE);

		removeAllSubgoals();

		std::list<sf::Vector2f> path;
		if (mOwner.getPathPlanner().createPathToPosition(mOwner.getPosition(), path))
		{
			addSubgoal<Goal_FollowPath>(mOwner, std::move(path));
		}
	}

	Goal<ZeldaEntity>::Status Goal_MoveToPosition::process(const sf::Time& dt)
	{
		if (isInactive())
		{
			activate();
		}

		Status status = processSubgoals(dt);
		setStatus(status);

		if (hasFailed())
		{
			activate();
		}

		return status;
	}

	void Goal_MoveToPosition::terminate() {}
}
