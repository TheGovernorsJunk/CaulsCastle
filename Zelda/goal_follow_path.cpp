#include "goal_follow_path.h"
#include "goal_seek_to_position.h"

namespace te
{
	Goal_FollowPath::Goal_FollowPath(ZeldaEntity& owner, const std::list<sf::Vector2f>& path)
		: mOwner(owner)
		, mPath(path)
	{}

	void Goal_FollowPath::activate()
	{
		setStatus(Status::ACTIVE);

		sf::Vector2f waypoint = mPath.front();
		mPath.pop_front();

		addSubgoal<Goal_SeekToPosition>(mOwner, waypoint);
	}

	Goal<ZeldaEntity>::Status Goal_FollowPath::process(const sf::Time& dt)
	{
		if (isInactive())
		{
			activate();
		}

		setStatus(processSubgoals(dt));

		if (isCompleted() && !mPath.empty())
		{
			activate();
		}

		return getStatus();
	}

	void Goal_FollowPath::terminate()
	{}
}
