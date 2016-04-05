#include "camera.h"
#include "base_game_entity.h"

namespace te
{
	Camera::Camera(const std::shared_ptr<BaseGameEntity>& pSubject, sf::Vector2f size)
		: mpSubject(pSubject)
		, mSize(size)
	{}

	sf::View Camera::getView(const sf::Transform& transform) const
	{
		sf::View view(transform.transformPoint(mpSubject->getPosition()), mSize);
		return view;
	}
}
