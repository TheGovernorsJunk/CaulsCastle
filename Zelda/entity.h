#ifndef TE_ENTITY_H
#define TE_ENTITY_H

#include "moving_entity.h"
#include "state_machine.h"
#include "steering_behaviors.h"

#include <SFML/Graphics.hpp>

#include <memory>

namespace te
{
	class MessageDispatcher;

	class Entity : public sf::Drawable, public MovingEntity
	{
	public:
		Entity(const std::shared_ptr<MessageDispatcher>&);
		void setSprite(const sf::Sprite& sprite);
		void setBoxCollider(const sf::FloatRect& boxCollider);
		void setDrawColliderEnabled(float enabled);

		void setVelocity(const sf::Vector2f& velocity);
		sf::Vector2f getVelocity() const;

		virtual void update(const sf::Time& dt);
		virtual bool handleMessage(const Telegram& msg);

		const SteeringBehaviors& getSteering() const;
		SteeringBehaviors& getSteering();
	private:
		virtual void draw(sf::RenderTarget&, sf::RenderStates) const;

		sf::Sprite mSprite;
		sf::FloatRect mBoxCollider;
		float mDrawColliderEnabled;
		sf::Vector2f mVelocity;

		StateMachine<Entity> mFSM;
		const std::shared_ptr<MessageDispatcher> mpMessageDispatcher;
		const std::shared_ptr<EntityManager> mpEntityManager;
		SteeringBehaviors mSteering;
	};
}

#endif
