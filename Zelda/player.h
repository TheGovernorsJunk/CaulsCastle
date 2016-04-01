#ifndef TE_PLAYER_H
#define TE_PLAYER_H

#include "base_game_entity.h"
#include "tmx.h"
#include "box_collider.h"

namespace te
{
	class Player : public BaseGameEntity, public Collidable
	{
	public:
		enum Message
		{
			Off   = 0x00,
			On    = 0x01,

			//Left  = 0x02,
			//Right = 0x04,
			//Up    = 0x08,
			//Down  = 0x10

			X     = 0x02,
			Y     = 0x04
		};

		Player(Game& world, const TMX::Object& playerObject);

		bool handleMessage(const Telegram& msg);
		void update(const sf::Time& dt);

		bool intersects(const BoxCollider&) const;
		bool intersects(const CompositeCollider&) const;

	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		float mRadius;
		sf::Vector2f mVelocity;
		BoxCollider mBoxCollider;
	};
}

#endif
