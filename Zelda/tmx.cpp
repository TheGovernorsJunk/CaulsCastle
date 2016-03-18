#include "tmx.h"

#include "texture_manager.h"
#include "tile_map.h"

#include <SFML/Graphics.hpp>
#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>

#include <memory>
#include <algorithm>
#include <array>

namespace te
{
	TMX::TMX(const std::string& filename)
	{
		rapidxml::file<> tmxFile(filename.c_str());
		rapidxml::xml_document<> tmx;
		tmx.parse<0>(tmxFile.data());

		rapidxml::xml_node<char>* pMapNode = tmx.first_node("map");
		mWidth = std::stoi(pMapNode->first_attribute("width")->value());
		mHeight = std::stoi(pMapNode->first_attribute("height")->value());
		mTilewidth = std::stoi(pMapNode->first_attribute("tilewidth")->value());
		mTileheight = std::stoi(pMapNode->first_attribute("tileheight")->value());

		for (rapidxml::xml_node<char>* pTileset = tmx.first_node("map")->first_node("tileset"); pTileset != 0; pTileset = pTileset->next_sibling("tileset"))
		{
			rapidxml::xml_node<char>* pImage = pTileset->first_node("image");
			mTilesets.push_back({
				std::stoi(pTileset->first_attribute("firstgid")->value()),
				pTileset->first_attribute("name")->value(),
				std::stoi(pTileset->first_attribute("tilewidth")->value()),
				std::stoi(pTileset->first_attribute("tileheight")->value()),
				std::stoi(pTileset->first_attribute("tilecount")->value()), {
					pImage->first_attribute("source")->value(),
					std::stoi(pImage->first_attribute("width")->value()),
				    std::stoi(pImage->first_attribute("height")->value())
				}
			});
		}

		for (rapidxml::xml_node<char>* pLayer = tmx.first_node("map")->first_node("layer"); pLayer != 0; pLayer = pLayer->next_sibling("layer"))
		{
			std::vector<Tile> tiles;
			for (rapidxml::xml_node<char>* pTile = pLayer->first_node("data")->first_node("tile"); pTile != 0; pTile = pTile->next_sibling("tile"))
			{
				tiles.push_back({
					std::stoi(pTile->first_attribute("gid")->value())
				});
			}
			mLayers.push_back({
				pLayer->first_attribute("name")->value(),
				std::stoi(pLayer->first_attribute("width")->value()),
				std::stoi(pLayer->first_attribute("height")->value()),
				{std::move(tiles)}
			});
		}
	}

	static std::vector<TMX::Tileset>::const_iterator getTilesetIterator(int gid, const std::vector<TMX::Tileset>& tilesets)
	{
		auto retIt = tilesets.end();
		for (auto it = tilesets.begin(); it != tilesets.end(); ++it)
		{
			if (gid >= it->firstgid && gid < it->firstgid + it->tilecount)
			{
				retIt = it;
			}
		}
		if (retIt == tilesets.end())
		{
			throw std::out_of_range("GID does not exist for TMX.");
		}
		return retIt;
	}

	TileMap TMX::makeTileMap(TextureManager& textureManager) const
	{
		std::vector<std::shared_ptr<sf::Texture>> pTextures;
		std::for_each(mTilesets.begin(), mTilesets.end(), [&pTextures, &textureManager](const Tileset& tileset) {
			pTextures.push_back(textureManager.get(tileset.image.source));
		});

		std::vector<std::vector<sf::VertexArray>> layers;
		std::for_each(mLayers.begin(), mLayers.end(), [this, &layers](const Layer& layer) {
			std::vector<sf::VertexArray> vertexArrays(mTilesets.size());
			std::for_each(vertexArrays.begin(), vertexArrays.end(), [](sf::VertexArray& va) {
				va.setPrimitiveType(sf::Quads);
			});

			for (auto tileIter = layer.data.tiles.begin(); tileIter != layer.data.tiles.end(); ++tileIter) {
				int tileIndex = tileIter - layer.data.tiles.begin();
				int x = tileIndex % mWidth;
				int y = tileIndex / mWidth;

				std::array<sf::Vertex,4> quad;
				quad[0].position = sf::Vector2f(x * mTilewidth, y * mTileheight);
				quad[1].position = sf::Vector2f((x + 1) * mTilewidth, y * mTileheight);
				quad[2].position = sf::Vector2f((x + 1) * mTilewidth, (y + 1) * mTileheight);
				quad[3].position = sf::Vector2f(x * mTilewidth, (y + 1) * mTileheight);

				auto tilesetIter = getTilesetIterator(tileIter->gid, mTilesets);
				int localId = tileIter->gid - tilesetIter->firstgid;
				int tu = localId % (tilesetIter->image.width / tilesetIter->tilewidth);
				int tv = localId / (tilesetIter->image.width / tilesetIter->tilewidth);

				quad[0].texCoords = sf::Vector2f(tu * mTilewidth, tv * mTileheight);
				quad[1].texCoords = sf::Vector2f((tu + 1) * mTilewidth, tv * mTileheight);
				quad[2].texCoords = sf::Vector2f((tu + 1) * mTilewidth, (tv + 1) * mTileheight);
				quad[3].texCoords = sf::Vector2f(tu * mTilewidth, (tv + 1) * mTileheight);

				int tilesetIndex = tilesetIter - mTilesets.begin();
				std::for_each(quad.begin(), quad.end(), [&vertexArrays, tilesetIndex](sf::Vertex& v) {
					vertexArrays[tilesetIndex].append(v);
				});
			}

			layers.push_back(vertexArrays);
		});

		return TileMap(pTextures, layers);
	}
}
