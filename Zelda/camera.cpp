#include "camera.h"
#include "base_game_entity.h"

namespace te
{
	Camera::Camera(const BaseGameEntity& subject, sf::Vector2f size)
		: mSubject(subject)
		, mSize(size)
	{}

	sf::View Camera::getView(const sf::Transform& transform) const
	{
		sf::View view(transform.transformPoint(mSubject.getPosition()), mSize);
		return view;
	}
}
