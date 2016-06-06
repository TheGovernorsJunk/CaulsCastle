#include "animator.h"
#include "texture_manager.h"
#include "sprite_renderer.h"

#include <cassert>

namespace te
{
	std::unique_ptr<Animator> Animator::make(TextureManager& tm, SpriteRenderer& sr)
	{
		return std::unique_ptr<Animator>(new Animator(tm, sr));
	}

	Animator::Animator(TextureManager& tm, SpriteRenderer& sr)
		: mTextureManager(tm)
		, mSpriteRenderer(sr)
		, mpAnimation(nullptr)
		, mCurrPlayTime(sf::Time::Zero)
	{}

	void Animator::setAnimation(TextureID animation)
	{
		mpAnimation = &mTextureManager.getAnimation(animation);

		sf::Time zero = sf::Time::Zero;
		mSpriteRenderer.setSprite(mpAnimation->getSprite(zero));
		mCurrPlayTime = zero;
	}

	void Animator::update(const sf::Time& dt)
	{
		if (!mpAnimation) return;

		mCurrPlayTime += dt;
		sf::Time duration = mpAnimation->getDuration();
		if (mCurrPlayTime >= duration) mCurrPlayTime -= duration;
		mSpriteRenderer.setSprite(mpAnimation->getSprite(mCurrPlayTime));
	}
}
