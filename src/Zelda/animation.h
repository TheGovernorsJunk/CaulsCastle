#ifndef TE_ANIMATION_H
#define TE_ANIMATION_H

#include "typedefs.h"
#include "texture_atlas.h"

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
		virtual ~Animation() {}

		const std::string& getID() const;
		virtual const sf::Sprite& getSprite(const sf::Time& dt) const = 0;
		virtual sf::Time getDuration() const = 0;

	private:
		struct Clip
		{
			std::string id;
			sf::Sprite sprite;
		};
		class FixedIntervalAnimation;
		class VariableIntervalAnimation;

		static std::unique_ptr<Animation> make(const std::string& animationID, int millisecondsPerClip, std::vector<Clip>&& clips);
		static std::unique_ptr<Animation> make(const TextureAtlas::Animation& animationData, const TextureManager& textureManager);

		Animation(const std::string& name);

		std::string mName;
	};
}

#endif
