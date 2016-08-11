#ifndef TE_GAME_DATA_H
#define TE_GAME_DATA_H

#include "types.h"
#include "xbox_controller.h"
#include "input.h"
#include "mesh.h"
#include "texture.h"
#include "resource_holder.h"

#include <boost/container/flat_map.hpp>

#include <memory>

class b2World;
class b2Body;

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
		vec2 pixel_to_world_scale;

		std::unique_ptr<b2World> physics_world;

		std::vector<Texture> textures;
		Resource_holder<Mesh<vec3, vec2>> meshes3;

		Entity_manager entity_manager;

		flat_map<Player_id, SDL_Joystick*> joysticks;
		flat_map<Player_id, Keymap> keymaps;
		flat_map<Player_id, Player_input> inputs;
		flat_map<Player_id, Entity_id> avatars;

		struct Body_deleter {
			Body_deleter(b2World& world);
			void operator()(b2Body*) const;
			b2World* p_world;
		};
		component<Entity_id, std::unique_ptr<b2Body, Body_deleter>> rigid_bodies;
		component<Entity_id, float> max_speeds;
		component<Entity_id, vec2> velocities;
		component<Entity_id, vec2> positions;

		template <typename Resource>
		struct Render_data {
			Resource_id<Resource> resource_id;
			glm::mat4 transform;
		};
		Multi_component<Entity_id, Render_data<Mesh<vec3, vec2>>> entity_meshes3;

		glm::mat4 view_matrix;

		Game_data();
		~Game_data();
	};
}

#endif
