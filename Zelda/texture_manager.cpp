#include "texture_manager.h"

#include <SFML/Graphics.hpp>

namespace te
{
	std::unique_ptr<TextureManager> TextureManager::make()
	{
		return std::unique_ptr<TextureManager>(new TextureManager);
	}

	TextureManager::TextureManager() {}

	static const std::hash<std::string> hashFn;
	TextureID TextureManager::getID(const std::string& filename)
	{
		return hashFn(filename);
	}

	TextureID TextureManager::load(const std::string& filename)
	{
		TextureID id = getID(filename);
		auto iter = mTextures.find(id);
		if (iter == mTextures.end())
		{
			auto texture = std::make_unique<sf::Texture>();
			if (!texture->loadFromFile(filename))
			{
				throw std::runtime_error("Texture file not found.");
			}
			mTextures.insert({ id, std::move(texture) });
			return id;
		}
		else
		{
			return iter->first;
		}
	}

	TextureID TextureManager::loadSpritesheet(const std::string& filename)
	{
		auto atlas = TextureAtlas::make(filename, this);
		TextureID textureID = atlas->getTextureID();
		std::vector<TextureAtlas::Sprite> sprites(atlas->getSpriteCount());
		atlas->insertSprites(sprites.begin());

		for (auto& s : sprites)
		{
			auto pSprite = std::make_unique<sf::Sprite>(getTexture(textureID), sf::IntRect(s.x, s.y, s.w, s.h));
			pSprite->setOrigin((float)s.pX * s.w, (float)s.pY * s.h);
			mSpriteMap.insert({ s.n, std::move(pSprite) });
		}

		return atlas->getTextureID();
	}

	void TextureManager::loadAnimations(const std::string& filename)
	{
		std::vector<Animation> animations = Animation::load(filename, *this);

		for (auto& anim : animations) mAnimationMap.insert({ anim.getID(), std::make_unique<Animation>(std::move(anim)) });
	}

	const sf::Texture& TextureManager::getTexture(TextureID file) const
	{
		auto iter = mTextures.find(file);
		if (iter != mTextures.end())
		{
			return *iter->second;
		}
		throw std::runtime_error("No texture for given ID.");
	}

	const sf::Sprite& TextureManager::getSprite(SpriteID sprite) const
	{
		auto iter = mSpriteMap.find(sprite);
		if (iter != mSpriteMap.end()) return *iter->second;
		throw std::runtime_error("No sprite for given ID.");
	}

	const Animation& TextureManager::getAnimation(AnimationID id) const
	{
		auto found = mAnimationMap.find(id);
		if (found != mAnimationMap.end()) return *found->second;
		throw std::runtime_error("No animation for given ID.");
	}
}
