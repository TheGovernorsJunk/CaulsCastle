#include "vector_ops.h"

namespace te
{
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
