#ifndef TE_CAMERA_H
#define TE_CAMERA_H

#include <SFML/Graphics.hpp>

#include <memory>

namespace te
{
	class BaseGameEntity;

	class Camera
	{
	public:
		Camera(const BaseGameEntity& pSubject, sf::Vector2f size);

		sf::View getView(const sf::Transform& transform = sf::Transform::Identity) const;

	private:
		const BaseGameEntity& mSubject;
		sf::Vector2f mSize;
	};
}

#endif
