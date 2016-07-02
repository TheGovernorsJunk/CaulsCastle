#include "animation.h"
#include "texture_manager.h"

#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>

namespace te
{
	const sf::Sprite& Animation::getSprite(const sf::Time& dt) const
	{
		size_t index = (size_t)(dt / mTimePerFrame) % mFrames.size();
		return mFrames[index].sprite;
	}
}
