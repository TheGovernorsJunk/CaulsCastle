#ifndef TE_ANIMATOR_H
#define TE_ANIMATOR_H

#include "typedefs.h"
#include "renderer.h"
#include "component.h"
#include "resource_manager.h"
#include "animation.h"

#include <SFML/System.hpp>

#include <memory>

namespace te
{
	class BaseGameEntity;
	class TextureManager;

	class Animator : public UpdateComponent
	{
	public:
		static std::unique_ptr<Animator> make(BaseGameEntity& owner);

		void setAnimation(ResourceID<Animation> animation);
		ResourceID<Animation> getAnimation() const { return mAnimationID; }
		void update(const sf::Time& dt);

	private:
		Animator(BaseGameEntity& owner);

		ResourceManager<Animation>& mAnimationManager;
		Renderer<sf::Sprite>* mpSpriteRenderer;

		ResourceID<Animation> mAnimationID;
		Animation* mpAnimation;
		sf::Time mCurrPlayTime;
	};
}

#endif
