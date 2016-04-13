#ifndef TE_PLAYER_H
#define TE_PLAYER_H

#include "base_game_entity.h"
#include "box_collider.h"
#include "tmx.h"

#include <Box2D/Box2D.h>

namespace te
{
	class ZeldaGame;
	class SpriteRenderer;
	class Animator;

	class Player : public BaseGameEntity
	{
	public:
		enum Message
		{
			Off   = 0x00,
			On    = 0x01,

			X     = 0x02,
			Y     = 0x04
		};

		static std::unique_ptr<Player> make(ZeldaGame& world, const TMX::Object& playerObject, sf::Transform transform = sf::Transform::Identity);

		bool handleMessage(const Telegram& msg);

	private:
		Player(ZeldaGame& world, const TMX::Object& playerObject, sf::Transform transform);

		void onUpdate(const sf::Time& dt);
		void onDraw(sf::RenderTarget& target, sf::RenderStates states) const;

		float mRadius;
		std::unique_ptr<b2Fixture, std::function<void(b2Fixture*)>> mpFixture;
		std::unique_ptr<SpriteRenderer> mpSpriteRenderer;
		std::unique_ptr<Animator> mpAnimator;
	};
}

#endif
