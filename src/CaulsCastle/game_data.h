#ifndef TE_GAME_DATA_H
#define TE_GAME_DATA_H

#include "types.h"
#include "xbox_controller.h"
#include "input.h"
#include "mesh.h"
#include "texture.h"

#include <boost/container/flat_map.hpp>

#include <memory>

namespace te
{
	struct Keymap {
		Xbox_axis x_movement;
		Xbox_axis y_movement;

		Xbox_button dodge;

		Keymap()
			: x_movement{ X_axis }
			, y_movement{ Y_axis }
			, dodge{ B_button }
		{}
	};

	class Entity_manager {
	public:
		Entity_manager();
		Entity_id get_free_id();
	private:
		Entity_id m_next_id;
	};

	struct Game_data {
		std::vector<Texture> textures;

		Entity_manager entity_manager;

		flat_map<Player_id, SDL_Joystick*> joysticks;
		flat_map<Player_id, Keymap> keymaps;
		flat_map<Player_id, Player_input> inputs;
		flat_map<Player_id, Entity_id> avatars;

		component<Entity_id, float> max_speeds;
		component<Entity_id, vec2> velocities;
		component<Entity_id, vec2> positions;
		Multi_component<Entity_id, Mesh<vec2, vec2>> meshes;
	};
}

#endif
