#include "vector_ops.h"

namespace te
{
	float length(const sf::Vector2f& v)
	{
		return std::sqrtf(v.x * v.x + v.y * v.y);
	}

	sf::Vector2f normalize(const sf::Vector2f& v)
	{
		return v / std::sqrtf(v.x * v.x + v.y * v.y);
	}

	float distanceSq(const sf::Vector2f& a, const sf::Vector2f& b)
	{
		float x = a.x - b.x;
		float y = a.y - b.y;
		return x * x + y * y;
	}
}
