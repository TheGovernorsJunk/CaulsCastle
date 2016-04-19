#ifndef TE_ANIMATION_H
#define TE_ANIMATION_H

#include "typedefs.h"

#include <SFML/Graphics.hpp>

#include <string>
#include <vector>
#include <list>

namespace te
{
	class TextureManager;

	class Animation
	{
	public:
		static std::vector<Animation> load(const std::string& filename, TextureManager& textureManager);

		TextureID getID() const;
		int getMillisecondsPerClip() const;
		const sf::Sprite& getSprite(size_t index) const;

	private:
		struct Clip
		{
			//TextureID texture;
			TextureID id;
			sf::Sprite sprite;
		};

		Animation(TextureID name, int millisecondsPerClip, std::vector<Clip>&& clips);

		TextureID mName;
		int mMillisecondsPerClip;
		std::vector<Clip> mClips;
	};
}

#endif
