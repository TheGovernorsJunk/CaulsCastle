#ifndef TE_SPRITE_RENDERER_H
#define TE_SPRITE_RENDERER_H

#include "texture_atlas.h"

#include <SFML/Graphics.hpp>

#include <memory>

namespace te
{
	class BaseGameEntity;

	class SpriteRenderer : public sf::Drawable
	{
	public:
		static std::unique_ptr<SpriteRenderer> make(BaseGameEntity&);

		void setSprite(std::shared_ptr<sf::Texture> pTexture, const TextureAtlas::Sprite& spriteInfo);
	private:
		SpriteRenderer(BaseGameEntity&);

		void draw(sf::RenderTarget&, sf::RenderStates) const;

		BaseGameEntity& mOwner;
		std::shared_ptr<sf::Texture> mpTexture;
		sf::Sprite mSprite;
	};
}

#endif
