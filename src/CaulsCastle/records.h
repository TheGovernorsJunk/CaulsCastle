#ifndef TE_RECORDS_H
#define TE_RECORDS_H

#include "texture_atlas.h"
#include "animation.h"
#include "resource_holder.h"
#include <rapidxml.hpp>
#include <string>
#include <algorithm>

namespace te {

struct Image_record {
	std::string filename;
	int width;
	int height;
};

struct Sprite_record {
	std::string filename;
	std::string image_filename;
	int x;
	int y;
	int w;
	int h;
	float px;
	float py;
};

struct Animation_record {
	std::string filename;
};

struct Animation_frame_record {
	std::string animation_filename;
	std::string sprite_filename;
	int delay;
	int delay_unit;
	size_t frame_index;
};

struct Animation_group_record {
	std::string name;
};

struct Animation_group_type_record {
	std::string group_name;
	std::string type;
	std::string animation_filename;
};

template <typename Resource>
struct Resource_record {
	std::string name;
	Resource_id<Resource> id;
};

struct Collider_record {
	std::string animation_name;
	int x;
	int y;
	int w;
	int h;
	size_t frame_start;
	size_t frame_end;
};

template <typename Image_table_iter, typename Sprite_table_iter>
void load_atlas(const std::string& filename,
		Image_table_iter image_out,
		Sprite_table_iter sprite_out)
{
	Texture_atlas atlas{ filename.c_str() };
	image_out++ = { atlas.imagePath, Image_record{
		atlas.imagePath,
		atlas.width,
		atlas.height
	} };

	for (auto& sprite : atlas.sprites) {
		sprite_out++ = { sprite.n, {
			sprite.n,
			atlas.imagePath,
			sprite.x,
			sprite.y,
			sprite.w,
			sprite.h,
			sprite.pX,
			sprite.pY
		} };
	}
}

template <typename Animation_table_iter, typename Animation_sprite_container_iter>
void load_animation(const std::string& dir,
		    const std::string& csv_filename,
		    Animation_table_iter animation_out,
		    Animation_sprite_container_iter animation_sprite_out)
{
	Animation_csv csv{ dir + csv_filename };
	animation_out++ = { csv_filename, { csv_filename } };

	size_t i = 0;
	for (auto& sprite : csv.frames) {
		animation_sprite_out++ = {
			csv_filename,
			sprite.name,
			sprite.delay,
			csv.delay_unit,
			i++
		};
	}
}

} // namespace te

#endif
