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

		//void setSprite(sf::Texture& texture, const TextureAtlas::Sprite& spriteInfo);
		void setSprite(const sf::Sprite& sprite);
	private:
		SpriteRenderer(BaseGameEntity&);

		void draw(sf::RenderTarget&, sf::RenderStates) const;

		BaseGameEntity& mOwner;
		sf::Sprite mSprite;
	};
}

#endif
