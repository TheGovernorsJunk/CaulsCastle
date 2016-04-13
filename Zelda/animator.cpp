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
		, mClipTime(sf::Time::Zero)
		, mCurrPlayTime(sf::Time::Zero)
		, mClipIndex(0)
	{}

	void Animator::setAnimation(TextureID animation)
	{
		mpAnimation = &mTextureManager.getAnimation(animation);

		sf::Sprite sprite = mpAnimation->getSprite(0);
		float width = sprite.getLocalBounds().width, height = sprite.getLocalBounds().height;
		mSpriteRenderer.setSprite(mpAnimation->getSprite(0));
		mSpriteRenderer.scale(1.f / width, 1.f / height);
		mSpriteRenderer.setOrigin(width / 2.f, height / 2.f);

		mClipTime = sf::milliseconds(mpAnimation->getMillisecondsPerClip());
		mCurrPlayTime = sf::Time::Zero;
		mClipIndex = 0;
	}

	void Animator::update(const sf::Time& dt)
	{
		if (!mpAnimation) return;

		mCurrPlayTime += dt;
		if (mCurrPlayTime >= mClipTime)
		{
			mSpriteRenderer.setSprite(mpAnimation->getSprite(++mClipIndex));
			mCurrPlayTime -= mClipTime;
		}
	}
}
