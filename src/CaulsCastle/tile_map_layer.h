#ifndef TE_TILE_MAP_LAYER
#define TE_TILE_MAP_LAYER

#include "types.h"
#include "tmx.h"

#include <vector>
#include <array>
#include <cassert>

namespace te
{
	template <typename Iter>
	void get_tile_map_layer_vertices(const tmx& tmx, size_t layer_index, Iter out)
	{
		assert(layer_index >= 0 && layer_index < tmx.layers.size());
		assert(tmx.orientation == tmx::Orientation::Orthogonal);

		auto tileWidth = tmx.tilewidth;
		auto tileHeight = tmx.tileheight;

		size_t tile_index = 0;
		const auto& layer = tmx.layers[layer_index];
		for (auto tile : layer.data)
		{
			if (tile.gid != 0)
			{
				int x = tile_index % tmx.width;
				int y = tile_index / tmx.width;

				std::array<vertex, 4> quad;
				quad[0].position = { (float)(tileWidth * x), (float)(tileHeight * y) };
				quad[1].position = { (float)(tileWidth * (x + 1)), (float)(tileHeight * y) };
				quad[2].position = { (float)(tileWidth * (x + 1)), (float)(tileHeight * (y + 1)) };
				quad[3].position = { (float)(tileWidth * x), (float)(tileHeight * (y + 1)) };

				size_t tilesetIndex = tmx.getTilesetIndex(tile.gid);
				const tmx::Tileset& tileset = tmx.tilesets[tilesetIndex];
				int localId = tile.gid - tileset.firstgid;
				int tu = localId % (tileset.image.width / tileset.tilewidth);
				int tv = localId / (tileset.image.width / tileset.tilewidth);

				quad[0].tex_coords = { (float)(tileset.tilewidth * tu) / tileset.image.width, (float)(tileset.tileheight * tv) / tileset.image.height };
				quad[1].tex_coords = { (float)(tileset.tilewidth * (tu + 1)) / tileset.image.width, (float)(tileset.tileheight * tv) / tileset.image.height };
				quad[2].tex_coords = { (float)(tileset.tilewidth * (tu + 1)) / tileset.image.width, (float)(tileset.tileheight * (tv + 1)) / tileset.image.height };
				quad[3].tex_coords = { (float)(tileset.tilewidth * tu) / tileset.image.width, (float)(tileset.tileheight * (tv + 1)) / tileset.image.height };

				//for (auto& v : quad) m_models[tileset_index].vertexArray.append(v);
			}
			++tile_index;
		}
	}
}

#endif
