#include "shape.h"

namespace te
{
	PolygonShape::PolygonShape(const b2PolygonShape& shape)
		: mPolygonShape(shape)
	{}

	CircleShape::CircleShape(sf::Vector2f center, float radius)
		: mCircleShape{}
	{
		mCircleShape.m_p.Set(center.x, center.y);
		mCircleShape.m_radius = radius;
	}

	AABB::AABB(sf::Vector2f lowerBound, sf::Vector2f upperBound)
		: mAABB{}
	{
		mAABB.lowerBound.Set(lowerBound.x, lowerBound.y);
		mAABB.upperBound.Set(upperBound.x, upperBound.y);
	}
}
