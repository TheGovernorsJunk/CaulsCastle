#ifndef TE_GAME_DATA_H
#define TE_GAME_DATA_H

#include "types.h"

namespace te
{
	struct Game_data {
		entity_id next_id;
		component<entity_id, vec2> positions;

		Game_data();
	};
}

#endif
