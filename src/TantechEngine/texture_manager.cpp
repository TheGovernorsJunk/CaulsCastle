#include "texture_manager.h"
#include "texture.h"

namespace te
{
    TextureManager::TextureManager() : mTextures() {}

    std::shared_ptr<Texture> TextureManager::operator[](const std::string& key)
    {
        auto it = mTextures.find(key);
        if (it != mTextures.end()) {
            return it->second;
        } else {
            std::shared_ptr<Texture> pTexture(new Texture{ key });
            mTextures.insert(std::pair<std::string, std::shared_ptr<Texture>>(key, pTexture));
            return pTexture;
        }
    }

    std::shared_ptr<Texture> TextureManager::operator[](const TMX::Tileset& tileset)
    {
        auto it = mTextures.find(tileset.image);
        if (it != mTextures.end()) {
            return it->second;
        } else {
            std::shared_ptr<Texture> pTexture(new Texture{ tileset });
            mTextures.insert(std::pair<std::string, std::shared_ptr<Texture>>(tileset.image, pTexture));
            return pTexture;
        }
    }
}
