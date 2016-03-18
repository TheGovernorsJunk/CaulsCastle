#ifndef TE_ENTITY_H
#define TE_ENTITY_H

#include "base_game_entity.h"
#include "state_machine.h"

#include <SFML/Graphics.hpp>

#include <memory>

namespace te
{
	class Entity : public sf::Drawable, public sf::Transformable, public BaseGameEntity
	{
	public:
		Entity();
		void setSprite(const sf::Sprite& sprite);
		void setBoxCollider(const sf::FloatRect& boxCollider);
		void setDrawColliderEnabled(float enabled);

		void setVelocity(const sf::Vector2f& velocity);
		sf::Vector2f getVelocity() const;

		virtual void update(const sf::Time& dt);
		virtual bool handleMessage(const Telegram& msg);
	private:
		virtual void draw(sf::RenderTarget&, sf::RenderStates) const;

		sf::Sprite mSprite;
		sf::FloatRect mBoxCollider;
		float mDrawColliderEnabled;
		sf::Vector2f mVelocity;

		StateMachine<Entity> mFSM;
	};
}

#endif
