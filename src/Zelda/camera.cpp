#include "camera.h"
#include "base_game_entity.h"
#include "entity_manager.h"

namespace te
{
	Camera::Camera(const EntityManager& em, int subjectID, sf::Vector2f size)
		: mEntityManager(em)
		, mSubjectID(subjectID)
		, mSize(size)
	{}

	sf::View Camera::getView(const sf::Transform& transform) const
	{
		if (mEntityManager.hasEntity(mSubjectID))
		{
			BaseGameEntity& subject = mEntityManager.getEntityFromID(mSubjectID);
			sf::View view(transform.transformPoint(subject.getWorldTransform().transformPoint(0, 0)), mSize);
			return view;
		}
		else
		{
			return sf::View();
		}
	}
}
