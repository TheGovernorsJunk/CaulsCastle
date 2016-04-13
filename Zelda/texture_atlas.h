#ifndef TE_TEXTURE_ATLAS_H
#define TE_TEXTURE_ATLAS_H

#include "typedefs.h"

#include <string>
#include <map>
#include <memory>

namespace te
{
	class TextureManager;

	class TextureAtlas
	{
	public:
		struct Sprite
		{
			float pX;
			float pY;
			int w;
			int h;
			int x;
			int y;
			TextureID n;
		};

		static std::unique_ptr<TextureAtlas> make(const std::string& dir, const std::string& filename, TextureManager* = nullptr);

		TextureID getTextureID() const;
		Sprite getSprite(TextureID id) const;

		size_t getSpriteCount() const;

		template <typename Iter>
		void insertSprites(Iter outIt)
		{
			for (auto spriteIt = mSprites.begin(); spriteIt != mSprites.end(); ++spriteIt)
			{
				*outIt = spriteIt->second;
				++outIt;
			}
		}

	private:
		TextureAtlas(const std::string& dir, const std::string& filename, TextureManager*);

		int mWidth;
		int mHeight;
		TextureID mImagePathID;
		std::map<TextureID, Sprite> mSprites;
	};
}

#endif
