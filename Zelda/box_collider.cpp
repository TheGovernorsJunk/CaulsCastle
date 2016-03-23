#include "box_collider.h"

#include "wall.h"

namespace te
{
	BoxCollider::BoxCollider(const sf::FloatRect& rect)
		: mRect(rect)
	{}

	std::vector<Wall2f> BoxCollider::getWalls() const
	{
		std::vector<Wall2f> walls;
		walls.push_back(Wall2f({ mRect.left, mRect.top }, { mRect.left, mRect.top + mRect.height }));
		walls.push_back(Wall2f({ mRect.left, mRect.top + mRect.height }, { mRect.left + mRect.width, mRect.top + mRect.height }));
		walls.push_back(Wall2f({ mRect.left + mRect.width, mRect.top + mRect.height }, { mRect.left + mRect.width, mRect.top }));
		walls.push_back(Wall2f({ mRect.left + mRect.width, mRect.top }, { mRect.left, mRect.top }));
		return walls;
	}

	void BoxCollider::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		sf::RectangleShape rect;
		rect.setPosition({ mRect.left, mRect.top });
		rect.setSize({ mRect.width, mRect.height });
		rect.setFillColor(sf::Color::Transparent);
		rect.setOutlineColor(sf::Color::Blue);
		rect.setOutlineThickness(1.f);
		target.draw(rect, states);
	}
}
