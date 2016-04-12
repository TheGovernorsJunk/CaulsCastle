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

	void SpriteRenderer::setSprite(sf::Texture& texture, const TextureAtlas::Sprite& spriteInfo)
	{
		mSprite.setTexture(texture);
		mSprite.setTextureRect(sf::IntRect(spriteInfo.x, spriteInfo.y, spriteInfo.w, spriteInfo.h));
	}

	void SpriteRenderer::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (mSprite.getTexture() != NULL)
		{
			states.transform *= getTransform();
			target.draw(mSprite, states);
		}
	}
}
