#ifndef TE_COLLIDER_H
#define TE_COLLIDER_H

#include <SFML/Graphics.hpp>

#include <vector>

namespace te
{
	class Wall2f;

	class Collider : public sf::Drawable
	{
	public:
		virtual ~Collider();

		virtual std::vector<Wall2f> getWalls() const = 0;
	};
}

#endif
