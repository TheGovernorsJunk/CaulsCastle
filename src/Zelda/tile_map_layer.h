#ifndef TE_TILE_MAP_LAYER_H
#define TE_TILE_MAP_LAYER_H

#include "tmx.h"

#include <SFML/Graphics.hpp>

#include <vector>
#include <algorithm>
#include <iterator>

namespace te
{
	class TileMapLayer : public sf::Drawable, public sf::Transformable
	{
	public:
		template <typename TextureIter, typename LayerOutIter>
		static void make(const TMX& tmx, TextureIter tilesetTexturesBegin, TextureIter tilesetTexturesEnd, LayerOutIter out)
		{
			std::vector<const sf::Texture*> textures;
			std::transform(tilesetTexturesBegin, tilesetTexturesEnd, std::back_inserter(textures), [](auto& texture) {
				return texture;
			});
			if (textures.size() != tmx.tilesetsSize()) throw std::runtime_error{"TileMapLayer::make: tileset count and texture count are not equal."};

			std::transform(tmx.layersBegin(), tmx.layersEnd(), out, [&tmx, &textures](const TMX::Layer& layer) {
				return TileMapLayer{tmx, layer, textures};
			});
		}

		TileMapLayer();

		unsigned getIndex() const { return m_Index; }

	private:
		TileMapLayer(const TMX& tmx, const TMX::Layer& layer, const std::vector<const sf::Texture*>& textures);
		void draw(sf::RenderTarget&, sf::RenderStates) const;

		struct Model
		{
			const sf::Texture* tilesetTexture;
			sf::VertexArray vertexArray;
		};
		unsigned m_Index;
		std::vector<Model> m_Models;
	};
}

#endif
