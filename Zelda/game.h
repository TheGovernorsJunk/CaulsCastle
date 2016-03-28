#ifndef TE_GAME_H
#define TE_GAME_H

#include <SFML/Graphics.hpp>

namespace te
{
	class Game
	{
	public:
		virtual ~Game();

		bool isPathObstructed(sf::Vector2f a, sf::Vector2f b, float boundingRadius = 0) const;
	};
}

#endif
