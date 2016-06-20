#ifndef TE_CAMERA_ENTITY_H
#define TE_CAMERA_ENTITY_H

#include "base_game_entity.h"

#include <memory>

namespace te
{
	class CameraEntity : public BaseGameEntity
	{
	public:
		static std::unique_ptr<CameraEntity> make(Game& world, sf::Vector2f position = {});
		sf::View getView() const;
		void setViewSize(float width, float height);
		//void setViewCenter(float width, float height);

	private:
		CameraEntity(Game& world, sf::Vector2f position);
		sf::View mView;
	};
}

#endif
