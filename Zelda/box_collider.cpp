#include "box_collider.h"
#include "composite_collider.h"

#include "wall.h"

#include <array>

namespace te
{
	BoxCollider::BoxCollider(const sf::FloatRect& rect)
		: mRect(rect)
		, mShape()
		, mWalls()
	{
		const b2Vec2 points[4]{
			b2Vec2{rect.left, rect.top},
			b2Vec2{rect.left + rect.width, rect.top},
			b2Vec2{rect.left + rect.width, rect.top + rect.height},
			b2Vec2{rect.left, rect.top + rect.height}
		};
		mShape.Set(points, 4);

		mWalls.push_back(Wall2f({ rect.left, rect.top }, { rect.left, rect.top + rect.height }));
		mWalls.push_back(Wall2f({ rect.left, rect.top + rect.height }, { rect.left + rect.width, rect.top + rect.height }));
		mWalls.push_back(Wall2f({ rect.left + rect.width, rect.top + rect.height }, { rect.left + rect.width, rect.top }));
		mWalls.push_back(Wall2f({ rect.left + rect.width, rect.top }, { rect.left, rect.top }));
	}

	const std::vector<Wall2f>& BoxCollider::getWalls() const
	{
		return mWalls;
	}

	bool BoxCollider::contains(float x, float y) const
	{
		b2Transform transform;
		transform.SetIdentity();
		return mShape.TestPoint(transform, b2Vec2(x, y));
	}

	bool BoxCollider::intersects(const BoxCollider& o) const
	{
		b2Transform transform;
		transform.SetIdentity();
		return b2TestOverlap(&mShape, 0, &o.mShape, 0, transform, transform);
	}

	bool BoxCollider::intersects(const BoxCollider& o, sf::FloatRect& collision) const
	{
		return mRect.intersects(o.mRect, collision);
	}

	bool BoxCollider::intersects(const CompositeCollider& o) const
	{
		return o.intersects(*this);
	}

	bool BoxCollider::intersects(const CompositeCollider& o, sf::FloatRect& collision) const
	{
		return o.intersects(*this, collision);
	}

	BoxCollider BoxCollider::transform(const sf::Transform& t) const
	{
		return { t.transformRect(mRect) };
	}

	sf::FloatRect BoxCollider::getRect() const
	{
		return mRect;
	}

	void BoxCollider::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		sf::RectangleShape rect;
		rect.setPosition({ mRect.left, mRect.top });
		rect.setSize({ mRect.width, mRect.height });
		sf::Color fillColor(255, 0, 0, 50);
		rect.setFillColor(fillColor);
		target.draw(rect, states);
	}
}
