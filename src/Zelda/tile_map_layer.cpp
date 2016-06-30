#include "tile_map_layer.h"

#include <array>

namespace te
{
	TileMapLayer::TileMapLayer(const TMX& tmx, const TMX::Layer& layer, const std::vector<const sf::Texture*>& textures)
		: m_Models{}
	{
		std::transform(textures.begin(), textures.end(), std::back_inserter(m_Models), [](const sf::Texture* pTexture) {
			return Model{pTexture, {}};
		});

		for (auto& model : m_Models) model.vertexArray.setPrimitiveType(sf::Quads);

		std::vector<const TMX::Tileset*> tilesets;
		std::transform(tmx.tilesetsBegin(), tmx.tilesetsEnd(), std::back_inserter(tilesets), [](const TMX::Tileset& tileset) {
			return &tileset;
		});

		TMX::Orientation orientation = tmx.getOrienation();
		if (orientation == TMX::Orientation::Orthogonal)
		{
			int width = tmx.getWidth();
			int height = tmx.getHeight();

			size_t tileIndex = 0;
			for (auto tile : layer.data)
			{
				if (tile.gid != 0)
				{
					int x = tileIndex % width;
					int y = tileIndex / width;

					std::array<sf::Vertex, 4> quad;
					quad[0].position = {(float)(width * x), (float)(height * y)};
					quad[1].position = {(float)(width * (x + 1)), (float)(height * y)};
					quad[2].position = {(float)(width * (x + 1)), (float)(height * (y + 1))};
					quad[3].position = {(float)(width * x), (float)(height * (y + 1))};

					size_t tilesetIndex = tmx.getTilesetIndex(tile.gid);
					const TMX::Tileset& tileset = *tilesets[tilesetIndex];
					int localId = tile.gid - tileset.firstgid;
					int tu = localId % (tileset.image.width / tileset.tilewidth);
					int tv = localId / (tileset.image.width / tileset.tilewidth);

					quad[0].texCoords = {(float)(tileset.tilewidth * tu), (float)(tileset.tileheight * tv)};
					quad[1].texCoords = {(float)(tileset.tilewidth * (tu + 1)), (float)(tileset.tileheight * tv)};
					quad[2].texCoords = {(float)(tileset.tilewidth * (tu + 1)), (float)(tileset.tileheight * (tv + 1))};
					quad[3].texCoords = {(float)(tileset.tilewidth * tu), (float)(tileset.tileheight * (tv + 1))};

					for (auto& v : quad) m_Models[tilesetIndex].vertexArray.append(v);
				}
				++tileIndex;
			}
		}
		else
		{
			throw std::runtime_error{"TileMapLayer ctor: Unsupported orientation."};
		}
	}

	void TileMapLayer::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		for (auto& model : m_Models)
		{
			states.texture = model.tilesetTexture;
			target.draw(model.vertexArray, states);
		}
	}
}
