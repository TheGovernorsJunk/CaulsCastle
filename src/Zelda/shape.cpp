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
}
