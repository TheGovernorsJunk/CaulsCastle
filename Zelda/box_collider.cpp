#include "box_collider.h"
#include "composite_collider.h"

#include "wall.h"

namespace te
{
	BoxCollider::BoxCollider(const sf::FloatRect& rect)
		: mRect(rect)
		, mWalls()
	{
		mWalls.push_back(Wall2f({ mRect.left, mRect.top }, { mRect.left, mRect.top + mRect.height }));
		mWalls.push_back(Wall2f({ mRect.left, mRect.top + mRect.height }, { mRect.left + mRect.width, mRect.top + mRect.height }));
		mWalls.push_back(Wall2f({ mRect.left + mRect.width, mRect.top + mRect.height }, { mRect.left + mRect.width, mRect.top }));
		mWalls.push_back(Wall2f({ mRect.left + mRect.width, mRect.top }, { mRect.left, mRect.top }));
	}

	const std::vector<Wall2f>& BoxCollider::getWalls() const
	{
		return mWalls;
	}

	bool BoxCollider::contains(float x, float y) const
	{
		return mRect.contains(x, y);
	}

	bool BoxCollider::intersects(const BoxCollider& o) const
	{
		return mRect.intersects(o.mRect);
	}

	bool BoxCollider::intersects(const CompositeCollider& o) const
	{
		return o.intersects(*this);
	}

	//std::vector<Wall2f> BoxCollider::getWalls() const
	//{
	//	std::vector<Wall2f> walls;
	//	walls.push_back(Wall2f({ mRect.left, mRect.top }, { mRect.left, mRect.top + mRect.height }));
	//	walls.push_back(Wall2f({ mRect.left, mRect.top + mRect.height }, { mRect.left + mRect.width, mRect.top + mRect.height }));
	//	walls.push_back(Wall2f({ mRect.left + mRect.width, mRect.top + mRect.height }, { mRect.left + mRect.width, mRect.top }));
	//	walls.push_back(Wall2f({ mRect.left + mRect.width, mRect.top }, { mRect.left, mRect.top }));
	//	return walls;
	//}

	BoxCollider BoxCollider::transform(const sf::Transform& t) const
	{
		return { t.transformRect(mRect) };
	}

	void BoxCollider::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		sf::RectangleShape rect;
		rect.setPosition({ mRect.left, mRect.top });
		rect.setSize({ mRect.width, mRect.height });
		sf::Color fillColor(255, 0, 0, 50);
		rect.setFillColor(fillColor);
		//rect.setOutlineColor(sf::Color::Blue);
		//rect.setOutlineThickness(1.f);
		target.draw(rect, states);
	}
}
