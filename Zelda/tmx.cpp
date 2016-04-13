#include "tmx.h"

#include "texture_manager.h"
#include "tile_map.h"
#include "composite_collider.h"
#include "nav_graph_node.h"
#include "nav_graph_edge.h"
#include "vector_ops.h"

#include <SFML/Graphics.hpp>
#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>

#include <algorithm>
#include <array>
#include <set>
#include <cmath>
#include <sstream>

constexpr bool std::less<sf::Vector2f>::operator()(const sf::Vector2f& a, const sf::Vector2f& b) const
{
	return a.x < b.x || (a.x == b.x && a.y < b.y);
}

constexpr bool std::less<te::NavGraphEdge>::operator()(const te::NavGraphEdge& a, const te::NavGraphEdge& b) const
{
	return a.getFrom() < b.getFrom() ||
		(a.getFrom() == b.getFrom() && a.getTo() < b.getTo()) ||
		(a.getFrom() == b.getFrom() && a.getTo() == b.getTo() && a.getCost() < b.getCost());
}

namespace te
{
	const int TMX::NULL_TILE = -1;
	const TMX::TileData TMX::NULL_DATA = TMX::TileData{ NULL_TILE, TMX::ObjectGroup() };

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
			std::vector<TileData> tiles;
			for (rapidxml::xml_node<char>* pTile = pTileset->first_node("tile"); pTile != 0; pTile = pTile->next_sibling("tile"))
			{
				rapidxml::xml_node<char>* pObjectGroup = pTile->first_node("objectgroup");
				std::vector<Object> objects;
				for (rapidxml::xml_node<char>* pObject = pObjectGroup->first_node("object"); pObject != 0; pObject = pObject->next_sibling("object"))
				{
					std::vector<Polygon> polygons;
					for (rapidxml::xml_node<char>* pPolygon = pObject->first_node("polygon"); pPolygon != 0; pPolygon = pPolygon->next_sibling("polygon"))
					{
						std::vector<sf::Vector2i> pointsVec;

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
							pointsVec.push_back({ std::stoi(xCoord), std::stoi(yCoord) });
						}

						polygons.push_back({ std::move(pointsVec) });
					}

					objects.push_back(Object{
						std::stoi(pObject->first_attribute("id")->value()),
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
						std::move(objects)
					}
				});
			}

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
				},
				std::move(tiles)
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

		for (rapidxml::xml_node<char>* pObjectgroup = tmx.first_node("map")->first_node("objectgroup"); pObjectgroup != 0; pObjectgroup = pObjectgroup->next_sibling("objectgroup"))
		{
			std::vector<Object> objects;
			for (rapidxml::xml_node<char>* pObject = pObjectgroup->first_node("object"); pObject != 0; pObject = pObject->next_sibling("object"))
			{
				objects.push_back({
					std::stoi(pObject->first_attribute("id")->value()),
					pObject->first_attribute("name") != 0 ? pObject->first_attribute("name")->value() : 0,
					std::stoi(pObject->first_attribute("x")->value()),
					std::stoi(pObject->first_attribute("y")->value()),
					pObject->first_attribute("width") != 0 ? std::stoi(pObject->first_attribute("width")->value()) : 0,
					pObject->first_attribute("height") != 0 ? std::stoi(pObject->first_attribute("height")->value()) : 0
				});
			}
			mObjectGroups.push_back({
				pObjectgroup->first_attribute("name")->value(),
				"",
				std::move(objects)
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

	void TMX::makeVertices(TextureManager& textureManager, std::vector<const sf::Texture*>& textures, std::vector<std::vector<sf::VertexArray>>& layers, int widthUnitsPerTile, int heightUnitsPerTile) const
	{
		if (widthUnitsPerTile < 0 || heightUnitsPerTile < 0) throw std::runtime_error("Invalid units per tile passed to TMX::makeVertices.");
		if (widthUnitsPerTile == 0) widthUnitsPerTile = mTilewidth;
		if (heightUnitsPerTile == 0) heightUnitsPerTile = mTileheight;

		textures.clear();
		std::for_each(mTilesets.begin(), mTilesets.end(), [&textures, &textureManager](const Tileset& tileset) {
			textures.push_back(&textureManager.getTexture(textureManager.load(tileset.image.source)));
		});

		layers.clear();
		std::for_each(mLayers.begin(), mLayers.end(), [this, &layers, widthUnitsPerTile, heightUnitsPerTile](const Layer& layer) {
			std::vector<sf::VertexArray> vertexArrays(mTilesets.size());
			std::for_each(vertexArrays.begin(), vertexArrays.end(), [](sf::VertexArray& va) {
				va.setPrimitiveType(sf::Quads);
			});

			for (auto tileIter = layer.data.tiles.begin(); tileIter != layer.data.tiles.end(); ++tileIter) {
				if (tileIter->gid != 0)
				{
					int tileIndex = tileIter - layer.data.tiles.begin();
					int x = tileIndex % mWidth;
					int y = tileIndex / mWidth;

					std::array<sf::Vertex, 4> quad;
					quad[0].position = sf::Vector2f((float)x * widthUnitsPerTile, (float)y * heightUnitsPerTile);
					quad[1].position = sf::Vector2f((x + 1.f) * widthUnitsPerTile, (float)y * heightUnitsPerTile);
					quad[2].position = sf::Vector2f((x + 1.f) * widthUnitsPerTile, (y + 1.f) * heightUnitsPerTile);
					quad[3].position = sf::Vector2f((float)x * widthUnitsPerTile, (y + 1.f) * heightUnitsPerTile);

					auto tilesetIter = getTilesetIterator(tileIter->gid, mTilesets);
					int localId = tileIter->gid - tilesetIter->firstgid;
					int tu = localId % (tilesetIter->image.width / tilesetIter->tilewidth);
					int tv = localId / (tilesetIter->image.width / tilesetIter->tilewidth);

					quad[0].texCoords = sf::Vector2f((float)tu * mTilewidth, (float)tv * mTileheight);
					quad[1].texCoords = sf::Vector2f((tu + 1.f) * mTilewidth, (float)tv * mTileheight);
					quad[2].texCoords = sf::Vector2f((tu + 1.f) * mTilewidth, (tv + 1.f) * mTileheight);
					quad[3].texCoords = sf::Vector2f((float)tu * mTilewidth, (tv + 1.f) * mTileheight);

					int tilesetIndex = tilesetIter - mTilesets.begin();
					std::for_each(quad.begin(), quad.end(), [&vertexArrays, tilesetIndex](sf::Vertex& v) {
						vertexArrays[tilesetIndex].append(v);
					});
				}
			}

			layers.push_back(vertexArrays);
		});

		//TileMap tm(pTextures, layers);
	}

	const TMX::TileData& TMX::getTileData(int x, int y, const TMX::Layer& layer) const
	{
		int i = y * mWidth + x;
		if (i >= mWidth * mHeight)
		{
			throw std::out_of_range("Tile coordinates are out of bounds.");
		}
		const Tile& tile = layer.data.tiles[i];
		if (tile.gid == 0)
		{
			return TMX::NULL_DATA;
		}
		auto tilesetIter = getTilesetIterator(tile.gid, mTilesets);
		const int localId = tile.gid - tilesetIter->firstgid;
		const std::vector<TileData>& tiles = tilesetIter->tiles;
		auto result = std::find_if(tiles.begin(), tiles.end(), [localId](const TileData& data) {
			return localId == data.id;
		});
		if (result == tiles.end())
		{
			return TMX::NULL_DATA;
		}
		return *result;
	}

	CompositeCollider* TMX::makeCollider(const sf::Transform& sourceTransform) const
	{
		CompositeCollider* pCollider = new CompositeCollider();
		std::for_each(mLayers.begin(), mLayers.end(), [pCollider, &sourceTransform, this](const Layer& layer) {
			for (int y = 0; y < mHeight; ++y)
			{
				for (int x = 0; x < mWidth; ++x)
				{
					const TMX::TileData& tileData = getTileData(x, y, layer);
					if (tileData.id != NULL_TILE)
					{
						sf::Transform transform = sourceTransform;
						transform.translate((float)x * mTilewidth, (float)y * mTileheight);
						std::for_each(tileData.objectgroup.objects.begin(), tileData.objectgroup.objects.end(), [pCollider, &transform](const Object& obj) {
							if (obj.polygons.size() == 0) {
								pCollider->addCollider({ transform.transformRect({ (float)obj.x, (float)obj.y, (float)obj.width, (float)obj.height }) });
							}
						});
					}
				}
			}
		});
		return pCollider;
	}

	int TMX::index(int x, int y) const
	{
		return y * mWidth + x;
	}

	SparseGraph<NavGraphNode, NavGraphEdge>* TMX::makeNavGraph(const sf::Transform& transform) const
	{
		std::unique_ptr<CompositeCollider> pCollider(makeCollider(transform));
		NavGraphNode seedNode;
		seedNode.setIndex(-1);
		int x = 0, y = 0;
		while (seedNode.getIndex() == -1 && y < mHeight)
		{
			sf::Vector2f coords = transform.transformPoint({ x * mTilewidth + (mTilewidth / 2.f), y * mTileheight + (mTileheight / 2.f) });
			if (!pCollider->contains(coords.x, coords.y))
			{
				seedNode.setIndex(0);
				seedNode.setPosition(coords);
			}
			else
			{
				++x;
				if (x == mWidth)
				{
					x = 0;
					++y;
				}
			}
		}

		SparseGraph<NavGraphNode, NavGraphEdge>* pGraph = new SparseGraph<NavGraphNode, NavGraphEdge>();
		if (seedNode.getIndex() != -1)
		{
			int seedIndex = pGraph->addNode(seedNode);

			std::vector<NavGraphNode> allNodes;
			std::map<sf::Vector2f, int> assigned;
			assigned.insert(std::make_pair(pGraph->getNode(seedIndex).getPosition(), seedIndex));
			auto flood = [&, this](int startIndex) {
				sf::Vector2f pos = pGraph->getNode(startIndex).getPosition();
				std::vector<int> newIndices;

				const std::array<sf::Vector2f, 4> offsets = {
					transform.transformPoint(sf::Vector2f((float)mTilewidth, 0)),
					transform.transformPoint(sf::Vector2f(-(float)mTilewidth, 0)),
					transform.transformPoint(sf::Vector2f(0, (float)mTileheight)),
					transform.transformPoint(sf::Vector2f(0, -(float)mTileheight))
				};
				std::for_each(offsets.begin(), offsets.end(), [&](const sf::Vector2f& offset) {
					sf::Vector2f newPos = offset + pos;
					sf::Vector2f max = transform.transformPoint((float)mWidth * mTilewidth, (float)mHeight * mTileheight);
					if (assigned.find(newPos) == assigned.end() && !pCollider->contains(newPos.x, newPos.y) && newPos.x > 0 && newPos.x < max.x && newPos.y > 0 && newPos.y < max.y)
					{
						NavGraphNode newNode;
						newNode.setPosition(newPos);
						int newIndex = pGraph->addNode(newNode);
						//graph.addEdge(NavGraphEdge(startIndex, newIndex, length(graph.getNode(startIndex).getPosition() - newPos)));

						assigned.insert(std::make_pair(newPos, newIndex));
						newIndices.push_back(newIndex);
					}
				});

				// Add diagonal edges
				std::for_each(newIndices.begin(), newIndices.end(), [&](int newIndex) {
					sf::Vector2f newPosition = pGraph->getNode(newIndex).getPosition();

					const std::array<sf::Vector2f, 8> neighbors = {
						transform.transformPoint(sf::Vector2f((float)mTilewidth, 0)),
						transform.transformPoint(sf::Vector2f(-(float)mTilewidth, 0)),
						transform.transformPoint(sf::Vector2f(0, (float)mTileheight)),
						transform.transformPoint(sf::Vector2f(0, -(float)mTileheight)),
						transform.transformPoint(sf::Vector2f((float)mTilewidth, (float)mTileheight)),
						transform.transformPoint(sf::Vector2f(-(float)mTilewidth, (float)mTileheight)),
						transform.transformPoint(sf::Vector2f(-(float)mTilewidth, -(float)mTileheight)),
						transform.transformPoint(sf::Vector2f((float)mTilewidth, -(float)mTileheight))
					};
					std::for_each(neighbors.begin(), neighbors.end(), [&](sf::Vector2f offset) {
						sf::Vector2f neighbor = newPosition + offset;
						auto neighborIndexIter = assigned.find(neighbor);
						if (neighborIndexIter != assigned.end())
						{
							pGraph->addEdge(NavGraphEdge(newIndex, neighborIndexIter->second, length(pGraph->getNode(newIndex).getPosition() - neighbor)));
						}
					});
				});
				return newIndices;
			};

			auto flatMapFlood = [&](std::vector<int> indices) {
				std::vector<int> newIndices;
				std::for_each(indices.begin(), indices.end(), [&](int index) {
					std::vector<int> currIndices = flood(index);
					newIndices.insert(newIndices.end(), currIndices.begin(), currIndices.end());
				});
				return newIndices;
			};

			std::vector<int> newIndices = flatMapFlood({ seedIndex });
			while (newIndices.size() > 0)
			{
				newIndices = flatMapFlood(newIndices);
			}
		}
		pGraph->pruneEdges();
		return pGraph;
	}

	int TMX::getWidth() const
	{
		return mWidth;
	}

	int TMX::getHeight() const
	{
		return mHeight;
	}

	int TMX::getTileWidth() const
	{
		return mTilewidth;
	}

	int TMX::getTileHeight() const
	{
		return mTileheight;
	}

	std::vector<TMX::ObjectGroup> TMX::getObjectGroups() const
	{
		return mObjectGroups;
	}
}
