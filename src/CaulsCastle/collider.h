#ifndef TE_COLLIDER_H
#define TE_COLLIDER_H

#include "mesh.h"
#include "resource_holder.h"

namespace te {

struct Collider {
	Resource_id<Mesh2> mesh_id;
	int x;
	int y;
	int w;
	int h;
};

} // namespace te

#endif
