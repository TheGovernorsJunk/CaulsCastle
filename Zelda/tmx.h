#ifndef TE_TMX_H
#define TE_TMX_H

#include <string>
#include <vector>

namespace te
{
	class TextureManager;
	class TileMap;
	class CompositeCollider;

	class TMX
	{
	public:
		TMX(const std::string& filename);
		TileMap makeTileMap(TextureManager&) const;
		CompositeCollider makeCollider() const;
	private:
		struct Image {
			std::string source;
			int width;
			int height;
		};
		struct Object {
			int id;
			int x;
			int y;
			int width;
			int height;
		};
		struct ObjectGroup {
			std::string draworder;
			std::vector<Object> objects;
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

		int mWidth;
		int mHeight;
		int mTilewidth;
		int mTileheight;
		std::vector<Tileset> mTilesets;
		std::vector<Layer> mLayers;
	};
}

#endif
