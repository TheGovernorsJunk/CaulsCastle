#include "wall.h"
#include "vector_ops.h"
#include <cmath>

namespace te
{
	Wall2f::Wall2f(const sf::Vector2f& from, const sf::Vector2f& to)
		: mFrom(from)
		, mTo(to)
		, mNormal(normalize(sf::Vector2f(from.y - to.y, to.x - from.x)))
	{}

	sf::Vector2f Wall2f::getFrom() const
	{
		return mFrom;
	}

	sf::Vector2f Wall2f::getTo() const
	{
		return mTo;
	}

	sf::Vector2f Wall2f::getNormal() const
	{
		return mNormal;
	}
}
