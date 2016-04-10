#include "sprite_renderer.h"

namespace te
{
	std::unique_ptr<SpriteRenderer> SpriteRenderer::make(BaseGameEntity& owner)
	{
		return std::unique_ptr<SpriteRenderer>(new SpriteRenderer(owner));
	}

	SpriteRenderer::SpriteRenderer(BaseGameEntity& owner)
		: mOwner(owner)
		, mpTexture(nullptr)
		, mSprite()
	{}

	void SpriteRenderer::setSprite(std::shared_ptr<sf::Texture> pTexture, const TextureAtlas::Sprite& spriteInfo)
	{
		mpTexture = pTexture;
		mSprite.setTexture(*pTexture);
		mSprite.setTextureRect(sf::IntRect(spriteInfo.x, spriteInfo.y, spriteInfo.w, spriteInfo.h));
	}

	void SpriteRenderer::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		if (mpTexture)
		{
			target.draw(mSprite, states);
		}
	}
}
