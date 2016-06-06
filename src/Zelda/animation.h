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
		static std::vector<std::unique_ptr<Animation>> load(const std::string& filename, TextureManager& textureManager);

		template <typename Iter>
		static void load(const TextureAtlas& atlas, const TextureManager& textureManager, Iter out)
		{
			std::vector<TextureAtlas::Animation> animationData;
			atlas.getAnimations(std::back_inserter(animationData));
			std::transform(animationData.begin(), animationData.end(), out, [&textureManager](auto& anim) {
				return make(anim, textureManager);
			});
		}

		virtual ~Animation() {}

		TextureID getID() const;
		virtual const sf::Sprite& getSprite(const sf::Time& dt) const = 0;
		virtual sf::Time getDuration() const = 0;

	private:
		struct Clip
		{
			TextureID id;
			sf::Sprite sprite;
		};
		class FixedIntervalAnimation;
		class VariableIntervalAnimation;

		static std::unique_ptr<Animation> make(TextureID animationID, int millisecondsPerClip, std::vector<Clip>&& clips);
		static std::unique_ptr<Animation> make(const TextureAtlas::Animation& animationData, const TextureManager& textureManager);

		Animation(TextureID name);

		TextureID mName;
	};
}

#endif
