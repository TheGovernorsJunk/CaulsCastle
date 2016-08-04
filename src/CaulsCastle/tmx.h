#ifndef TE_TMX_H
#define TE_TMX_H

#include "types.h"

#include <string>
#include <vector>
#include <memory>
#include <algorithm>

namespace te
{
	struct tmx
	{
		using Index = size_t;

		enum class Orientation {
			Orthogonal,
			Isometric
		};

		struct Polygon {
			std::vector<vec2> points;
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
			friend struct tmx;
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

		bool loadFromFile(const std::string& filename);

		static const int NULL_TILE;
		static const TileData NULL_DATA;

		size_t getTilesetIndex(int gid) const;
		const TileData& getTileData(int x, int y, const Layer& layer) const;
		int index(int x, int y) const;

		std::string mFilename;
		Orientation mOrientation;
		int width;
		int height;
		int tilewidth;
		int tileheight;
		std::vector<Tileset> tilesets;
		std::vector<Layer> layers;
		std::vector<ObjectGroup> objectgroups;
		std::vector<std::string> layernames;
	};
}

#endif
