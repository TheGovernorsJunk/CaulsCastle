#ifndef TE_VECTOR_OPS_H
#define TE_VECTOR_OPS_H

#include <SFML/Graphics.hpp>

namespace te
{
	float length(const sf::Vector2f& v);
	sf::Vector2f normalize(const sf::Vector2f& v);
	float lengthSq(const sf::Vector2f& v);
	float distanceSq(const sf::Vector2f& a, const sf::Vector2f& b);
	sf::Vector2f truncate(const sf::Vector2f& v, float maxMagnitude);
	sf::Vector2f perp(const sf::Vector2f& v);
}

#endif
