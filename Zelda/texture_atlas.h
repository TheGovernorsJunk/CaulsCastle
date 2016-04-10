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
			int oW;
			int oH;
			int oX;
			int oY;
			int pX;
			int pY;
			int w;
			int h;
			int x;
			int y;
			TextureID n;
		};

		static std::unique_ptr<TextureAtlas> make(const std::string& dir, const std::string& filename, TextureManager* = nullptr);

		TextureID getTextureID() const;
		Sprite getSprite(TextureID id) const;

	private:
		TextureAtlas(const std::string& dir, const std::string& filename, TextureManager*);

		int mWidth;
		int mHeight;
		TextureID mImagePathID;
		std::map<TextureID, Sprite> mSprites;
	};
}

#endif
