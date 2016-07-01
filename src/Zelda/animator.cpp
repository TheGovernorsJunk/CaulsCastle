#include "animator.h"
#include "texture_manager.h"
#include "renderer.h"
#include "base_game_entity.h"
#include "game.h"

#include <cassert>

namespace te
{
	std::unique_ptr<Animator> Animator::make(BaseGameEntity& owner)
	{
		return std::unique_ptr<Animator>(new Animator(owner));
	}

	Animator::Animator(BaseGameEntity& owner)
		: mTextureManager(owner.getWorld().getTextureManager())
		, mpSpriteRenderer(nullptr)
		, mpAnimation(nullptr)
		, mCurrPlayTime(sf::Time::Zero)
	{
		if (auto* pRenderer = owner.getComponent<Renderer<sf::Sprite>>())
		{
			mpSpriteRenderer = pRenderer;
		}
		else
		{
			mpSpriteRenderer = &owner.addComponent<Renderer<sf::Sprite>>();
		}
	}

	void Animator::setAnimation(TextureID animation)
	{
		mpAnimation = &mTextureManager.getAnimation(animation);

		mpSpriteRenderer->setDrawable(mpAnimation->getSprite(sf::Time::Zero));
		mCurrPlayTime = sf::Time::Zero;
	}

	void Animator::update(const sf::Time& dt)
	{
		if (!mpAnimation) return;

		mCurrPlayTime += dt;
		sf::Time duration = mpAnimation->getDuration();
		if (mCurrPlayTime >= duration) mCurrPlayTime -= duration;
		mpSpriteRenderer->setDrawable(mpAnimation->getSprite(mCurrPlayTime));
	}
}
