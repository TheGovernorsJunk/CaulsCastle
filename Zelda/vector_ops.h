#ifndef TE_VECTOR_OPS_H
#define TE_VECTOR_OPS_H

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

namespace te
{
	float length(const sf::Vector2f& v);
	sf::Vector2f normalize(const sf::Vector2f& v);
	b2Vec2 normalize(const b2Vec2& v);
	float lengthSq(const sf::Vector2f& v);
	float lengthSq(const b2Vec2& v);
	float distance(const sf::Vector2f& a, const sf::Vector2f& b);
	float distanceSq(const sf::Vector2f& a, const sf::Vector2f& b);
	sf::Vector2f truncate(const sf::Vector2f& v, float maxMagnitude);
	sf::Vector2f perp(const sf::Vector2f& v);

	inline b2Vec2 operator*(const b2Vec2& v, float speed)
	{
		return b2Vec2(v.x * speed, v.y * speed);
	}
}

#endif
