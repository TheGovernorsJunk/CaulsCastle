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
		: mAnimationManager(owner.getWorld().getAnimationManager())
		, mpSpriteRenderer(nullptr)
		, mAnimationID{0}
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

	void Animator::setAnimation(ResourceID<Animation> id)
	{
		Animation* pAnimation = &mAnimationManager.get(id);
		if (pAnimation->getDuration() <= sf::Time::Zero) return;
		mpAnimation = pAnimation;
		mAnimationID = id;
		mpSpriteRenderer->setDrawable(mpAnimation->getSpriteID(sf::Time::Zero));
		mCurrPlayTime = sf::Time::Zero;
	}

	void Animator::update(const sf::Time& dt)
	{
		if (!mpAnimation) return;

		mCurrPlayTime += dt;
		sf::Time duration = mpAnimation->getDuration();
		if (mCurrPlayTime >= duration) mCurrPlayTime -= duration;
		mpSpriteRenderer->setDrawable(mpAnimation->getSpriteID(mCurrPlayTime));
	}
}
