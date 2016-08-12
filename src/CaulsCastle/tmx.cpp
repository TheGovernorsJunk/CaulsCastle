#include "tmx.h"

#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>

#include <algorithm>
#include <array>
#include <set>
#include <cmath>
#include <sstream>

namespace te {

const int Tmx::NULL_TILE = -1;
const Tmx::TileData Tmx::NULL_DATA = Tmx::TileData{ NULL_TILE, Tmx::ObjectGroup() };

Tmx::Tmx(const std::string& filename)
	: Tmx{}
{
	loadFromFile(filename);
}

bool Tmx::loadFromFile(const std::string& f)
{
	rapidxml::file<> tmxFile(f.c_str());
	rapidxml::xml_document<> tmx;
	tmx.parse<0>(tmxFile.data());

	filename = f;

	rapidxml::xml_node<char>* pMapNode = tmx.first_node("map");

	std::string orientationStr = pMapNode->first_attribute("orientation")->value();
	if (orientationStr == "orthogonal") orientation = Orientation::Orthogonal;
	else if (orientationStr == "isometric") orientation = Orientation::Isometric;
	else assert(false);

	width = std::stoi(pMapNode->first_attribute("width")->value());
	height = std::stoi(pMapNode->first_attribute("height")->value());
	tilewidth = std::stoi(pMapNode->first_attribute("tilewidth")->value());
	tileheight = std::stoi(pMapNode->first_attribute("tileheight")->value());

	for (rapidxml::xml_node<char>* pTileset = tmx.first_node("map")->first_node("tileset"); pTileset != 0; pTileset = pTileset->next_sibling("tileset"))
	{
		std::vector<TileData> tiles;
		for (rapidxml::xml_node<char>* pTile = pTileset->first_node("tile"); pTile != 0; pTile = pTile->next_sibling("tile"))
		{
			rapidxml::xml_node<char>* pObjectGroup = pTile->first_node("objectgroup");
			if (pObjectGroup != 0)
			{
				std::vector<Object> objects;
				for (rapidxml::xml_node<char>* pObject = pObjectGroup->first_node("object"); pObject != 0; pObject = pObject->next_sibling("object"))
				{
					std::vector<Polygon> polygons;
					for (rapidxml::xml_node<char>* pPolygon = pObject->first_node("polygon"); pPolygon != 0; pPolygon = pPolygon->next_sibling("polygon"))
					{
						decltype(polygons[0].points) pointsVec{};

						std::string pointsStr(pPolygon->first_attribute("points")->value());
						std::istringstream iss(pointsStr);
						std::vector<std::string> points{ std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{} };
						for (auto& point : points)
						{
							std::stringstream ss(point);
							std::string xCoord;
							std::getline(ss, xCoord, ',');
							std::string yCoord;
							std::getline(ss, yCoord);
							pointsVec.push_back({ std::stof(xCoord), std::stof(yCoord) });
						}

						polygons.push_back({ std::move(pointsVec) });
					}

					objects.push_back(Object{
						std::stoi(pObject->first_attribute("id")->value()),
						"",
						"",
						std::stoi(pObject->first_attribute("x")->value()),
						std::stoi(pObject->first_attribute("y")->value()),
						pObject->first_attribute("width") != 0 ? std::stoi(pObject->first_attribute("width")->value()) : 0,
						pObject->first_attribute("height") != 0 ? std::stoi(pObject->first_attribute("height")->value()) : 0,
						std::move(polygons)
					});
				}

				tiles.push_back({
					std::stoi(pTile->first_attribute("id")->value()),
					ObjectGroup {
						"",
						pObjectGroup->first_attribute("draworder")->value(),
						std::move(objects),
						0
					}
				});
			}
			else
			{
				tiles.push_back({
					std::stoi(pTile->first_attribute("id")->value()),
					ObjectGroup {}
				});
			}
		}

		rapidxml::xml_node<char>* pImage = pTileset->first_node("image");

		tilesets.push_back({
			std::stoi(pTileset->first_attribute("firstgid")->value()),
			pTileset->first_attribute("name")->value(),
			std::stoi(pTileset->first_attribute("tilewidth")->value()),
			std::stoi(pTileset->first_attribute("tileheight")->value()),
			std::stoi(pTileset->first_attribute("tilecount")->value()), {
				pImage->first_attribute("source")->value(),
				std::stoi(pImage->first_attribute("width")->value()),
			    std::stoi(pImage->first_attribute("height")->value())
			},
			std::move(tiles)
		});
	}

	rapidxml::xml_node<char>* pLastTileset = nullptr;
	for (auto* pTileset = tmx.first_node("map")->first_node("tileset"); pTileset != 0; pTileset = pTileset->next_sibling("tileset"))
		pLastTileset = pTileset;
	Index currIndex = 0;
	std::vector<Index> tileLayerIndices;
	std::vector<Index> objectLayerIndices;
	for (auto* pLayer = pLastTileset->next_sibling(); pLayer != 0; pLayer = pLayer->next_sibling())
	{
		if (std::string(pLayer->name()) == "layer")
			tileLayerIndices.push_back(currIndex);
		if (std::string(pLayer->name()) == "objectgroup")
			objectLayerIndices.push_back(currIndex);
		++currIndex;
	}

	auto layerIndexIter = tileLayerIndices.begin();
	for (rapidxml::xml_node<char>* pLayer = tmx.first_node("map")->first_node("layer"); pLayer != 0; pLayer = pLayer->next_sibling("layer"))
	{
		std::vector<Tile> tiles;
		for (rapidxml::xml_node<char>* pTile = pLayer->first_node("data")->first_node("tile"); pTile != 0; pTile = pTile->next_sibling("tile"))
		{
			tiles.push_back({
				std::stoi(pTile->first_attribute("gid")->value())
			});
		}
		layers.push_back({
			pLayer->first_attribute("name")->value(),
			std::stoi(pLayer->first_attribute("width")->value()),
			std::stoi(pLayer->first_attribute("height")->value()),
			{std::move(tiles)},
			*layerIndexIter++
		});
	}

	auto objectIndexIter = objectLayerIndices.begin();
	for (rapidxml::xml_node<char>* pObjectgroup = tmx.first_node("map")->first_node("objectgroup"); pObjectgroup != 0; pObjectgroup = pObjectgroup->next_sibling("objectgroup"))
	{
		std::vector<Object> objects;
		for (rapidxml::xml_node<char>* pObject = pObjectgroup->first_node("object"); pObject != 0; pObject = pObject->next_sibling("object"))
		{
			objects.push_back({
				std::stoi(pObject->first_attribute("id")->value()),
				pObject->first_attribute("name") != 0 ? pObject->first_attribute("name")->value() : "",
				pObject->first_attribute("type") != 0 ? pObject->first_attribute("type")->value() : "",
				std::stoi(pObject->first_attribute("x")->value()),
				std::stoi(pObject->first_attribute("y")->value()),
				pObject->first_attribute("width") != 0 ? std::stoi(pObject->first_attribute("width")->value()) : 0,
				pObject->first_attribute("height") != 0 ? std::stoi(pObject->first_attribute("height")->value()) : 0
			});
		}
		objectgroups.push_back({
			pObjectgroup->first_attribute("name")->value(),
			"",
			std::move(objects),
			*objectIndexIter++
		});
	}

	// This is some accidental duplication
	rapidxml::xml_node<char>* pLayerOrObjectNode = tmx.first_node("map")->first_node("tileset")->next_sibling();
	while (pLayerOrObjectNode != 0)
	{
		std::string nodeName{ pLayerOrObjectNode->name() };
		if (nodeName == "layer" || nodeName == "objectgroup")
		{
			layernames.push_back(pLayerOrObjectNode->first_attribute("name")->value());
		}
		pLayerOrObjectNode = pLayerOrObjectNode->next_sibling();
	}

	return true;
}

size_t Tmx::getTilesetIndex(int gid) const
{
	auto retIt = tilesets.end();
	for (auto it = tilesets.begin(); it != tilesets.end(); ++it)
	{
		if (gid >= it->firstgid && gid < it->firstgid + it->tilecount)
		{
			retIt = it;
		}
	}
	assert(retIt != tilesets.end());
	return retIt - tilesets.begin();
}

const Tmx::TileData& Tmx::getTileData(int x, int y, const Tmx::Layer& layer) const
{
	int i = y * width + x;
	assert(i < width * height);
	const Tile& tile = layer.data.tiles[i];
	if (tile.gid == 0)
	{
		return Tmx::NULL_DATA;
	}
	const Tileset& tileset = tilesets[getTilesetIndex(tile.gid)];
	const int localId = tile.gid - tileset.firstgid;
	const std::vector<TileData>& tiles = tileset.tiles;
	auto result = std::find_if(tiles.begin(), tiles.end(), [localId](const TileData& data) {
		return localId == data.id;
	});
	if (result == tiles.end())
	{
		return Tmx::NULL_DATA;
	}
	return *result;
}

int Tmx::index(int x, int y) const
{
	return y * width + x;
}

} //  namespace te
