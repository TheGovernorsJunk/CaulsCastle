#ifndef TE_TMX_H
#define TE_TMX_H

#include "sparse_graph.h"

#include <string>
#include <vector>
#include <memory>
#include <algorithm>

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
		using Index = size_t;

		enum class Orientation {
			Orthogonal,
			Isometric
		};

		struct Polygon {
			std::vector<sf::Vector2i> points;
		};
		struct Object {
			int id;
			std::string name;
			std::string type;
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
			Index index;
		};
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
		class Data {
			friend class TMX;
			std::vector<Tile> tiles;
			Data(std::vector<Tile>&& ts) : tiles(std::move(ts)) {}
		public:
			using ConstTileIterator = std::vector<Tile>::const_iterator;
			ConstTileIterator begin() const { return tiles.begin(); }
			ConstTileIterator end() const { return tiles.end(); }
		};
		struct Layer {
			std::string name;
			int width;
			int height;
			Data data;
			Index index;
		};

		TMX();
		bool loadFromFile(const std::string& filename);
		//void makeVertices(TextureManager& textureManager, std::vector<const sf::Texture*>& textures, std::vector<std::vector<sf::VertexArray>>& layers, std::vector<int>& drawOrders) const;
		CompositeCollider* makeCollider(const sf::Transform& transform = sf::Transform::Identity) const;

		SparseGraph<NavGraphNode, NavGraphEdge>* makeNavGraph(const sf::Transform& transform = sf::Transform::Identity) const;

		Orientation getOrienation() const;
		int getWidth() const;
		int getHeight() const;
		int getTileWidth() const;
		int getTileHeight() const;
		sf::Transform getTileToPixelTransform() const;

		std::vector<ObjectGroup> getObjectGroups() const;

		template <typename Iter>
		void getLayerNames(Iter out) const
		{
			std::transform(mLayerNames.begin(), mLayerNames.end(), out, [](auto& name) { return name; });
		}

		using ConstLayerIterator = std::vector<Layer>::const_iterator;
		ConstLayerIterator layersBegin() const { return mLayers.cbegin(); }
		ConstLayerIterator layersEnd() const { return mLayers.cend(); }

		using ConstTilesetIterator = std::vector<Tileset>::const_iterator;
		ConstTilesetIterator tilesetsBegin() const { return mTilesets.begin(); }
		ConstTilesetIterator tilesetsEnd() const { return mTilesets.end(); }
		size_t tilesetsSize() const { return mTilesets.size(); }
		size_t getTilesetIndex(int tileIndex) const;

		const std::string& getFilename() const { return mFilename; }

	private:
		static const int NULL_TILE;
		static const TileData NULL_DATA;

		//std::vector<Tileset>::const_iterator getTilesetIterator(int gid) const;
		const TileData& getTileData(int x, int y, const Layer& layer) const;
		int index(int x, int y) const;

		std::string mFilename;
		Orientation mOrientation;
		int mWidth;
		int mHeight;
		int mTilewidth;
		int mTileheight;
		std::vector<Tileset> mTilesets;
		std::vector<Layer> mLayers;
		std::vector<ObjectGroup> mObjectGroups;
		std::vector<std::string> mLayerNames;
	};
}

#endif
