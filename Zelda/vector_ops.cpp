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

	float lengthSq(const sf::Vector2f& v)
	{
		return v.x * v.x + v.y * v.y;
	}

	float distanceSq(const sf::Vector2f& a, const sf::Vector2f& b)
	{
		float x = a.x - b.x;
		float y = a.y - b.y;
		return x * x + y * y;
	}

	sf::Vector2f truncate(const sf::Vector2f& v, float maxMagnitude)
	{
		if (lengthSq(v) > (maxMagnitude * maxMagnitude))
		{
			return normalize(v) * maxMagnitude;
		}
		else
		{
			return v;
		}
	}

	sf::Vector2f perp(const sf::Vector2f& v)
	{
		return sf::Vector2f(-v.y, v.x);
	}
}
