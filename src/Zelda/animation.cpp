#include "animation.h"
#include "texture_manager.h"

#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>

namespace te
{
	Animation::Animation()
		: mFrames{}
		, mTimePerFrame{sf::Time::Zero}
		, mDuration{sf::Time::Zero}
	{}

	ResourceID<sf::Sprite> Animation::getSpriteID(const sf::Time& dt) const
	{
		size_t index = (size_t)(dt / mTimePerFrame) % mFrames.size();
		return mFrames[index].spriteID;
	}
}
