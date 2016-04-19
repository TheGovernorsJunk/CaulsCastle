#include "goal_seek_to_position.h"
#include "zelda_entity.h"
#include "vector_ops.h"

namespace te
{
	Goal_SeekToPosition::Goal_SeekToPosition(ZeldaEntity& owner, sf::Vector2f position)
		: mOwner(owner)
		, mPosition(position)
	{}

	void Goal_SeekToPosition::activate()
	{
		setStatus(Status::ACTIVE);
		mOwner.getSteering().setSeekEnabled(true, mPosition);
	}

	Goal<ZeldaEntity>::Status Goal_SeekToPosition::process(const sf::Time& dt)
	{
		if (isInactive())
			activate();

		sf::Vector2f currPosition = mOwner.getPosition();
		if (distanceSq(currPosition, mPosition) < 64.f)
		{
			setStatus(Status::COMPLETED);
		}

		return getStatus();
	}

	void Goal_SeekToPosition::terminate()
	{}
}
