#ifndef TE_ANIMATION_H
#define TE_ANIMATION_H

#include "typedefs.h"
#include "texture_atlas.h"
#include "resource_manager.h"

#include <SFML/Graphics.hpp>

#include <string>
#include <vector>
#include <list>
#include <iterator>
#include <algorithm>
#include <memory>

namespace te
{
	class TextureManager;

	class Animation
	{
	public:
		struct Frame
		{
			ResourceID<sf::Sprite> spriteID;
		};
		template<class InputIt>
		Animation(InputIt first, InputIt last, sf::Time timePerFrame)
			: mFrames{first, last}
			, mTimePerFrame{timePerFrame}
			, mDuration{sf::Time::Zero}
		{
			for (auto& frame : mFrames) mDuration += timePerFrame;
		}
		Animation::Animation();

		ResourceID<sf::Sprite> getSpriteID(const sf::Time& dt) const;
		const sf::Time& getDuration() const { return mDuration; }

	private:
		std::vector<Frame> mFrames;
		sf::Time mTimePerFrame;
		sf::Time mDuration;
	};
}

#endif
