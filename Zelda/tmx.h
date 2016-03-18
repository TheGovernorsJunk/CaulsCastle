#ifndef TE_TMX_H
#define TE_TMX_H

#include <string>
#include <vector>

namespace te
{
	class TextureManager;
	class TileMap;

	class TMX
	{
	public:
		TMX(const std::string& filename);
		TileMap makeTileMap(TextureManager&) const;
	private:
		struct Image {
			std::string source;
			int width;
			int height;
		};
		struct Tileset {
			int firstgid;
			std::string name;
			int tilewidth;
			int tileheight;
			int tilecount;
			Image image;
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

		//std::vector<Tileset>::const_iterator getTilesetIterator(int gid) const;
		friend std::vector<Tileset>::const_iterator getTilesetIterator(int, const std::vector<Tileset>&);

		int mWidth;
		int mHeight;
		int mTilewidth;
		int mTileheight;
		std::vector<Tileset> mTilesets;
		std::vector<Layer> mLayers;
	};
}

#endif
