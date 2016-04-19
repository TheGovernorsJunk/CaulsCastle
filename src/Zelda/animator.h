#ifndef TE_ANIMATOR_H
#define TE_ANIMATOR_H

#include "typedefs.h"

#include <SFML/System.hpp>

#include <memory>

namespace te
{
	class TextureManager;
	class SpriteRenderer;
	class Animation;

	class Animator
	{
	public:
		static std::unique_ptr<Animator> make(TextureManager&, SpriteRenderer&);

		void setAnimation(TextureID animation);
		void update(const sf::Time& dt);

	private:
		Animator(TextureManager&, SpriteRenderer&);

		TextureManager& mTextureManager;
		SpriteRenderer& mSpriteRenderer;

		const Animation* mpAnimation;
		sf::Time mClipTime;
		sf::Time mCurrPlayTime;
		size_t mClipIndex;
	};
}

#endif
