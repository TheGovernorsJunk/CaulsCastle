#ifndef TE_TEXTURE_ATLAS_H
#define TE_TEXTURE_ATLAS_H

#include "typedefs.h"

#include <string>
#include <map>
#include <memory>
#include <vector>
#include <algorithm>

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
			std::string n;
			bool r;
		};
		struct Clip
		{
			int index;
			std::string spriteID;
		};
		struct Animation
		{
			std::string id;
			std::vector<Clip> clips;
		};

		TextureAtlas();
		bool loadFromFile(const std::string& filename);

		//TextureID getTextureID() const;
		const std::string& getImagePath() const { return mImagePath; }
		Sprite getSprite(const std::string& id) const;

		size_t getSpriteCount() const;

		template <typename Iter>
		void insertSprites(Iter outIt) const
		{
			for (auto spriteIt = mSprites.begin(); spriteIt != mSprites.end(); ++spriteIt)
			{
				*outIt = spriteIt->second;
				++outIt;
			}
		}

		template <typename Iter>
		void getAnimations(Iter out) const
		{
			std::transform(mAnimations.begin(), mAnimations.end(), out, [](auto& kv) {
				return kv.second;
			});
		}

	private:

		int mWidth;
		int mHeight;
		std::string mImagePath;
		std::map<std::string, Sprite> mSprites;
		std::map<std::string, Animation> mAnimations;
	};
}

#endif
