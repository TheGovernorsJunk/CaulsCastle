#ifndef TE_TILE_MAP_LAYER
#define TE_TILE_MAP_LAYER

#include "types.h"
#include "tmx.h"
#include "utilities.h"

#include <vector>
#include <array>
#include <cassert>

namespace te
{
	template <template <typename container_type> typename Iter, typename container_type, typename denom_fn = pow2up_fn>
	void get_tile_map_layer_vertices(const tmx& tmx, size_t layer_index, size_t tileset_index, Iter<container_type> out, const denom_fn& denom_fn = denom_fn())
	{
		assert(layer_index >= 0 && layer_index < tmx.layers.size());
		assert(tileset_index >= 0 && tileset_index < tmx.tilesets.size());
		assert(tmx.orientation == tmx::Orientation::Orthogonal);

		auto tileWidth = tmx.tilewidth;
		auto tileHeight = tmx.tileheight;
		const tmx::Tileset& tileset = tmx.tilesets[tileset_index];
		auto image_width = denom_fn(tileset.image.width);
		auto image_height = denom_fn(tileset.image.height);

		size_t tile_index = 0;
		const auto& layer = tmx.layers[layer_index];
		for (auto tile : layer.data)
		{
			if (tile.gid != 0 && tmx.getTilesetIndex(tile.gid) == tileset_index)
			{
				std::array<container_type::value_type, 4> quad;
				using coord_t = decltype(container_type::value_type::value_type::x);

				int localId = tile.gid - tileset.firstgid;
				int tu = localId % (tileset.image.width / tileset.tilewidth);
				int tv = localId / (tileset.image.width / tileset.tilewidth);

				quad[0].tex_coords = { (coord_t)(tileset.tilewidth * tu) / image_width,
						       (coord_t)(tileset.tileheight * tv) / image_height };
				quad[1].tex_coords = { (coord_t)(tileset.tilewidth * (tu + 1)) / image_width,
						       (coord_t)(tileset.tileheight * tv) / image_height };
				quad[2].tex_coords = { (coord_t)(tileset.tilewidth * (tu + 1)) / image_width,
						       (coord_t)(tileset.tileheight * (tv + 1)) / image_height };
				quad[3].tex_coords = { (coord_t)(tileset.tilewidth * tu) / image_width,
						       (coord_t)(tileset.tileheight * (tv + 1)) / image_height };

				int x = tile_index % tmx.width;
				int y = tile_index / tmx.width;

				quad[0].position = { (coord_t)(tileWidth * x),
						     (coord_t)(tileHeight * y) };
				quad[1].position = { (coord_t)(tileWidth * (x + 1)),
						     (coord_t)(tileHeight * y) };
				quad[2].position = { (coord_t)(tileWidth * (x + 1)),
						     (coord_t)(tileHeight * (y + 1)) };
				quad[3].position = { (coord_t)(tileWidth * x),
						     (coord_t)(tileHeight * (y + 1)) };

				for (auto& v : quad) (out++) = v;
			}
			++tile_index;
		}
	}

	struct pow2up_fn {
		inline int operator()(int x) const
		{
			return pow2up(x);
		}
	};
}

#endif
