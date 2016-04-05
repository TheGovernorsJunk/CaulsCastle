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
		Camera(const std::shared_ptr<BaseGameEntity>& pSubject, sf::Vector2f size);

		sf::View getView(const sf::Transform& transform = sf::Transform::Identity) const;

	private:
		std::shared_ptr<BaseGameEntity> mpSubject;
		sf::Vector2f mSize;
	};
}

#endif
