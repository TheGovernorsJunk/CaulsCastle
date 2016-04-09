#ifndef TE_TEXTURE_ATLAS_H
#define TE_TEXTURE_ATLAS_H

#include "typedefs.h"

#include <string>
#include <map>

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

		TextureAtlas(const std::string& filename, TextureManager* = nullptr);

		TextureID getTextureID() const;
		Sprite getSprite(TextureID id) const;

	private:
		int mWidth;
		int mHeight;
		TextureID mImagePathID;
		std::map<TextureID, Sprite> mSprites;
	};
}

#endif
