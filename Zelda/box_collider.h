#ifndef TE_BOX_COLLIDER_H
#define TE_BOX_COLLIDER_H

#include "collider.h"
#include "wall.h"

namespace te
{
	class BoxCollider : public Collider
	{
	public:
		BoxCollider(const sf::FloatRect& rect);

		const std::vector<Wall2f>& getWalls() const;

		bool contains(float x, float y) const;

		bool intersects(const BoxCollider&) const;
		bool intersects(const BoxCollider&, sf::FloatRect& collision) const;
		bool intersects(const CompositeCollider&) const;
		bool intersects(const CompositeCollider&, sf::FloatRect& collision) const;

		BoxCollider transform(const sf::Transform&) const;
	private:
		virtual void draw(sf::RenderTarget&, sf::RenderStates) const;

		sf::FloatRect mRect;
		std::vector<Wall2f> mWalls;
	};
}

#endif
