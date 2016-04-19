#include "sprite_renderer.h"

namespace te
{
	std::unique_ptr<SpriteRenderer> SpriteRenderer::make(BaseGameEntity& owner)
	{
		return std::unique_ptr<SpriteRenderer>(new SpriteRenderer(owner));
	}

	SpriteRenderer::SpriteRenderer(BaseGameEntity& owner)
		: mOwner(owner)
		, mSprite()
	{}

	void SpriteRenderer::setSprite(const sf::Sprite& sprite)
	{
		mSprite = sprite;
	}

	void SpriteRenderer::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (mSprite.getTexture() != NULL)
		{
			target.draw(mSprite, states);
		}
	}
}
