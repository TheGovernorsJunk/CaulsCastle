#ifndef TE_TEXTURE_MANAGER_H
#define TE_TEXTURE_MANAGER_H

#include <map>
#include <string>
#include <memory>

namespace te
{
    class Texture;

    class TextureManager {
    public:
        TextureManager();

        std::shared_ptr<Texture> operator[](const std::string&);

    private:
        std::map<std::string, std::shared_ptr<Texture>> mTextures;

        TextureManager(const TextureManager&) = delete;
        TextureManager& operator=(const TextureManager&) = delete;
    };
}

#endif
