#ifndef TE_CAMERA_H
#define TE_CAMERA_H

#include <SFML/Graphics.hpp>

#include <memory>

namespace te
{
	class BaseGameEntity;
	class EntityManager;

	class Camera
	{
	public:
		Camera(const EntityManager& em, int subjectID, sf::Vector2f size);

		sf::View getView(const sf::Transform& transform = sf::Transform::Identity) const;

	private:
		const EntityManager& mEntityManager;
		int mSubjectID;
		sf::Vector2f mSize;
	};
}

#endif
