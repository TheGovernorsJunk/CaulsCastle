#ifndef TE_COLLIDER_H
#define TE_COLLIDER_H

#include <SFML/Graphics.hpp>

#include <vector>

namespace te
{
	class Wall2f;
	class BoxCollider;
	class CompositeCollider;

	class Collidable
	{
	public:
		virtual ~Collidable();
		virtual bool intersects(const BoxCollider&) const = 0;
		virtual bool intersects(const BoxCollider&, sf::FloatRect& collision) const = 0;
		virtual bool intersects(const CompositeCollider&) const = 0;
		virtual bool intersects(const CompositeCollider&, sf::FloatRect& collision) const = 0;
	};

	class Collider : public sf::Drawable, public Collidable
	{
	public:
		virtual ~Collider();

		virtual const std::vector<Wall2f>& getWalls() const = 0;
	};
}

#endif
