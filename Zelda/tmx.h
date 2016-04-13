#ifndef TE_TMX_H
#define TE_TMX_H

#include "sparse_graph.h"

#include <string>
#include <vector>
#include <memory>

class b2Shape;

namespace te
{
	class TextureManager;
	class TileMap;
	class CompositeCollider;
	class NavGraphNode;
	class NavGraphEdge;

	class TMX
	{
	public:
		struct Polygon {
			std::vector<sf::Vector2i> points;
		};
		struct Object {
			int id;
			std::string name;
			int x;
			int y;
			int width;
			int height;
			std::vector<Polygon> polygons;
		};
		struct ObjectGroup {
			std::string name;
			std::string draworder;
			std::vector<Object> objects;
		};

		TMX(const std::string& filename);
		void makeVertices(TextureManager& textureManager, std::vector<const sf::Texture*>& textures, std::vector<std::vector<sf::VertexArray>>& layers) const;
		CompositeCollider* makeCollider(const sf::Transform& transform = sf::Transform::Identity) const;

		SparseGraph<NavGraphNode, NavGraphEdge>* makeNavGraph(const sf::Transform& transform = sf::Transform::Identity) const;

		int getWidth() const;
		int getHeight() const;
		int getTileWidth() const;
		int getTileHeight() const;

		std::vector<ObjectGroup> getObjectGroups() const;

	private:
		struct Image {
			std::string source;
			int width;
			int height;
		};
		struct TileData {
			int id;
			ObjectGroup objectgroup;
		};
		struct Tileset {
			int firstgid;
			std::string name;
			int tilewidth;
			int tileheight;
			int tilecount;
			Image image;
			std::vector<TileData> tiles;
		};
		struct Tile {
			int gid;
		};
		struct Data {
			std::vector<Tile> tiles;
		};
		struct Layer {
			std::string name;
			int width;
			int height;
			Data data;
		};

		static const int NULL_TILE;
		static const TileData NULL_DATA;

		//std::vector<Tileset>::const_iterator getTilesetIterator(int gid) const;
		friend std::vector<Tileset>::const_iterator getTilesetIterator(int, const std::vector<Tileset>&);
		const TileData& getTileData(int x, int y, const Layer& layer) const;
		int index(int x, int y) const;

		int mWidth;
		int mHeight;
		int mTilewidth;
		int mTileheight;
		std::vector<Tileset> mTilesets;
		std::vector<Layer> mLayers;
		std::vector<ObjectGroup> mObjectGroups;
	};
}

#endif
