#include "goal_follow_path.h"

namespace te
{
	Goal_FollowPath::Goal_FollowPath(ZeldaEntity& owner, const std::list<sf::Vector2f>& path)
		: mOwner(owner)
		, mPath(path)
	{}

	void Goal_FollowPath::activate()
	{}

	Goal<ZeldaEntity>::Status Goal_FollowPath::process(const sf::Time& dt)
	{
		return Status::ACTIVE;
	}

	void Goal_FollowPath::terminate()
	{}
}
