#ifndef TE_ANIMATOR_H
#define TE_ANIMATOR_H

#include "typedefs.h"
#include "renderer.h"

#include <SFML/System.hpp>

#include <memory>

namespace te
{
	class TextureManager;
	class Animation;

	class Animator
	{
	public:
		static std::unique_ptr<Animator> make(TextureManager&, Renderer<sf::Sprite>&);

		void setAnimation(TextureID animation);
		void update(const sf::Time& dt);

	private:
		Animator(TextureManager&, Renderer<sf::Sprite>&);

		TextureManager& mTextureManager;
		Renderer<sf::Sprite>& mSpriteRenderer;

		const Animation* mpAnimation;
		sf::Time mCurrPlayTime;
	};
}

#endif
