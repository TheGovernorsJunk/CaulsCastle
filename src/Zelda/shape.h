#ifndef TE_SHAPE_H
#define TE_SHAPE_H

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

namespace te
{
	class Shape
	{
	public:
		~Shape() {}
		virtual const b2Shape& getShape() const = 0;
	};

	class PolygonShape : public Shape
	{
	public:
		PolygonShape(const b2PolygonShape& shape);
		const b2PolygonShape& getShape() const { return mPolygonShape; }

	private:
		b2PolygonShape mPolygonShape;
	};

	class CircleShape : public Shape
	{
	public:
		CircleShape(sf::Vector2f center, float radius);
		const b2CircleShape& getShape() const { return mCircleShape; }

	private:
		b2CircleShape mCircleShape;
	};
}

#endif
