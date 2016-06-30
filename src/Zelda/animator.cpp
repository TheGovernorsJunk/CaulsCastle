#include "animator.h"
#include "texture_manager.h"
#include "renderer.h"

#include <cassert>

namespace te
{
	std::unique_ptr<Animator> Animator::make(TextureManager& tm, Renderer<sf::Sprite>& sr)
	{
		return std::unique_ptr<Animator>(new Animator(tm, sr));
	}

	Animator::Animator(TextureManager& tm, Renderer<sf::Sprite>& sr)
		: mTextureManager(tm)
		, mSpriteRenderer(sr)
		, mpAnimation(nullptr)
		, mCurrPlayTime(sf::Time::Zero)
	{}

	void Animator::setAnimation(TextureID animation)
	{
		mpAnimation = &mTextureManager.getAnimation(animation);

		mSpriteRenderer.setDrawable(mpAnimation->getSprite(sf::Time::Zero));
		mCurrPlayTime = sf::Time::Zero;
	}

	void Animator::update(const sf::Time& dt)
	{
		if (!mpAnimation) return;

		mCurrPlayTime += dt;
		sf::Time duration = mpAnimation->getDuration();
		if (mCurrPlayTime >= duration) mCurrPlayTime -= duration;
		mSpriteRenderer.setDrawable(mpAnimation->getSprite(mCurrPlayTime));
	}
}
