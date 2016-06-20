#include "camera_entity.h"

namespace te
{
	std::unique_ptr<CameraEntity> CameraEntity::make(Game& world, sf::Vector2f position)
	{
		return std::unique_ptr<CameraEntity>{new CameraEntity{world, position}};
	}

	CameraEntity::CameraEntity(Game& world, sf::Vector2f position)
		: BaseGameEntity(world, position)
	{}

	sf::View CameraEntity::getView() const
	{
		sf::View view = mView;
		view.setCenter(getPosition());
		return view;
	}

	void CameraEntity::setViewSize(float width, float height)
	{
		mView.setSize(width, height);
	}

	//void CameraEntity::setViewCenter(float width, float height)
	//{
	//	mView.setCenter(width, height);
	//}
}
