#ifndef TE_VECTOR_OPS_H
#define TE_VECTOR_OPS_H

#include <SFML/Graphics.hpp>

namespace te
{
	float length(const sf::Vector2f& v);
	sf::Vector2f normalize(const sf::Vector2f& v);
	float distanceSq(const sf::Vector2f& a, const sf::Vector2f& b);
}

#endif
