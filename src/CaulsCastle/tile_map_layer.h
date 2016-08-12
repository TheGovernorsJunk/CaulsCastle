#ifndef TE_TILE_MAP_LAYER
#define TE_TILE_MAP_LAYER

#include "types.h"
#include "tmx.h"
#include "texture.h"
#include "utilities.h"

#include <vector>
#include <array>
#include <cassert>

namespace te {

template <template <typename Container_type> typename Iter, typename Container_type, typename Denom_fn = Pow2up_fn>
void get_tile_map_layer_vertices(const Tmx& tmx, size_t layer_index, size_t tileset_index, Iter<Container_type> out, int z_step = 500, const Denom_fn& denom_fn = Denom_fn())
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
			using vec_t = Container_type::value_type::position_value_type;
			using coord_t = decltype(Container_type::value_type::position_value_type::x);

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

			quad[0].position = detail::make_position<vec_t, coord_t>((coord_t)(tileWidth * x),
				(coord_t)(tileHeight * y),
										 (coord_t)layer_index * z_step);
			quad[1].position = detail::make_position<vec_t, coord_t>((coord_t)(tileWidth * (x + 1)),
				(coord_t)(tileHeight * y),
										 (coord_t)layer_index * z_step);
			quad[2].position = detail::make_position<vec_t, coord_t>((coord_t)(tileWidth * (x + 1)),
				(coord_t)(tileHeight * (y + 1)),
										 (coord_t)layer_index * z_step);
			quad[3].position = detail::make_position<vec_t, coord_t>((coord_t)(tileWidth * x),
				(coord_t)(tileHeight * (y + 1)),
										 (coord_t)layer_index * z_step);

			for (auto& v : quad) (out++) = v;
		}
		++tile_index;
	}
}

namespace detail
{
template <typename Vec, typename Coord>
inline Vec make_position(Coord x, Coord y, Coord z);
template <>
inline vec2 make_position(float x, float y, float z)
{
	return vec2{ x, y };
}
template <>
inline vec3 make_position(float x, float y, float z)
{
	return vec3{ x, y, z };
}

const auto load_texture = [](const std::string& texture_path)
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

template <typename Fn>
void iterate_layers_and_tilesets(const Tmx& tmx, Fn fn)
{
	for (size_t layer_i = 0; layer_i < tmx.layers.size(); ++layer_i) {
		for (size_t tileset_i = 0; tileset_i < tmx.tilesets.size(); ++tileset_i) {
			fn(layer_i, tileset_i);
		}
	}
}

struct Pow2up_fn {
	inline int operator()(int x) const
	{
		return pow2up(x);
	}
};

} // namespace te

#endif
