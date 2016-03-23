#ifndef TE_WALL_H
#define TE_WALL_H

#include <SFML/Graphics.hpp>

namespace te
{
	class Wall2f
	{
	public:
		Wall2f(const sf::Vector2f& from, const sf::Vector2f& to);

		sf::Vector2f getFrom() const;
		sf::Vector2f getTo() const;
		sf::Vector2f getNormal() const;
	private:
		sf::Vector2f mFrom;
		sf::Vector2f mTo;
		sf::Vector2f mNormal;
	};
}

#endif
