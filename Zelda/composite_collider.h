#ifndef TE_COMPOSITE_COLLIDER_H
#define TE_COMPOSITE_COLLIDER_H

#include "collider.h"
#include "box_collider.h"

#include <vector>

namespace te
{
	class BoxCollider;

	class CompositeCollider : public Collider
	{
	public:
		void addCollider(const BoxCollider& collider);
		virtual std::vector<Wall2f> getWalls() const;
	private:
		virtual void draw(sf::RenderTarget&, sf::RenderStates) const;
		std::vector<BoxCollider> mBoxColliders;
	};
}

#endif
