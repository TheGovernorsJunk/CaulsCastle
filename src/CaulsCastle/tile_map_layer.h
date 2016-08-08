#ifndef TE_TILE_MAP_LAYER
#define TE_TILE_MAP_LAYER

#include "types.h"
#include "tmx.h"
#include "texture.h"
#include "utilities.h"

#include <vector>
#include <array>
#include <cassert>

namespace te
{
	template <template <typename Container_type> typename Iter, typename Container_type, typename Denom_fn = Pow2up_fn>
	void get_tile_map_layer_vertices(const Tmx& tmx, size_t layer_index, size_t tileset_index, Iter<Container_type> out, const Denom_fn& denom_fn = Denom_fn())
	{
		assert(layer_index >= 0 && layer_index < tmx.layers.size());
		assert(tileset_index >= 0 && tileset_index < tmx.tilesets.size());
		assert(tmx.orientation == Tmx::Orientation::Orthogonal);

		auto tileWidth = tmx.tilewidth;
		auto tileHeight = tmx.tileheight;
		const Tmx::Tileset& tileset = tmx.tilesets[tileset_index];
		auto image_width = denom_fn(tileset.image.width);
		auto image_height = denom_fn(tileset.image.height);

		size_t tile_index = 0;
		const auto& layer = tmx.layers[layer_index];
		for (auto tile : layer.data)
		{
			if (tile.gid != 0 && tmx.getTilesetIndex(tile.gid) == tileset_index)
			{
				std::array<Container_type::value_type, 4> quad;
				using coord_t = decltype(Container_type::value_type::value_type::x);

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

	namespace detail
	{
		auto load_texture = [](const std::string& texture_path)
		{
			return load_texture32(texture_path);
		};
	}

	template <typename Iter, typename Load_fn = decltype(detail::load_texture)>
	void load_tileset_textures(const Tmx& tmx, Iter out, const Load_fn& load_fn = detail::load_texture)
	{
		auto dir = get_directory(tmx.filename);
		for (auto& tileset : tmx.tilesets) {
			out++ = load_fn(dir + tileset.image.source);
		}
	}

	struct Pow2up_fn {
		inline int operator()(int x) const
		{
			return pow2up(x);
		}
	};
}

#endif
