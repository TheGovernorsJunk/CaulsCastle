#include "entity.h"
#include "message_dispatcher.h"
#include "state.h"

#include <iostream>

namespace te
{
	// Dummy class for testing
	class EntityState : public State<Entity>
	{
	public:
		virtual void execute(Entity& entity, const sf::Time& dt)
		{
			//entity.move(dt.asSeconds() * entity.getVelocity());
			entity.updateOnForce(dt, entity.getSteering().calculate());
		}
		virtual bool onMessage(Entity& entity, const Telegram& telegram)
		{
			if (telegram.msg == 50)
			{
				std::cout << "Message received!" << std::endl;
				return true;
			}
			if (telegram.msg == 75)
			{
				//entity.getSteering().setSeekEnabled(true, sf::Vector2f(16 * 15.f, 16 * 20.f));
				//entity.getSteering().setFleeEnabled(true, sf::Vector2f(16 * 0.f, 16 * 0.f), 16 * 16.f);
				entity.getSteering().setArriveEnabled(true, sf::Vector2f(16 * 15.f, 16 * 20.f), SteeringBehaviors::Deceleration::Slow);
				return true;
			}
			return false;
		}
	};



	Entity::Entity(const std::shared_ptr<MessageDispatcher>& pMessageDispatcher)
		: sf::Drawable()
		, MovingEntity(64.f, 50.f, 90.f)
		, mSprite()
		, mBoxCollider()
		, mDrawColliderEnabled(false)
		, mVelocity(0, 0)
		, mFSM(*this, std::make_shared<EntityState>())
		, mpMessageDispatcher(pMessageDispatcher)
		, mpEntityManager(pMessageDispatcher->getEntityManager())
		, mSteering(*this)
	{}

	void Entity::setSprite(const sf::Sprite& sprite)
	{
		mSprite = sprite;
	}

	void Entity::setBoxCollider(const sf::FloatRect& boxCollider)
	{
		mBoxCollider = boxCollider;
	}

	void Entity::setDrawColliderEnabled(float enabled)
	{
		mDrawColliderEnabled = enabled;
	}

	void Entity::setVelocity(const sf::Vector2f& velocity)
	{
		mVelocity = velocity;
	}

	sf::Vector2f Entity::getVelocity() const
	{
		return mVelocity;
	}

	void Entity::update(const sf::Time& dt)
	{
		mFSM.update(dt);
	}

	bool Entity::handleMessage(const Telegram& telegram)
	{
		return mFSM.handleMessage(telegram);
	}

	const SteeringBehaviors& Entity::getSteering() const
	{
		return mSteering;
	}

	SteeringBehaviors& Entity::getSteering()
	{
		return mSteering;
	}

	void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		target.draw(mSprite, states);

		if (mDrawColliderEnabled)
		{
			sf::RectangleShape rect;
			rect.setSize({ mBoxCollider.width, mBoxCollider.height });
			rect.setPosition({ mBoxCollider.left, mBoxCollider.top });
			rect.setFillColor(sf::Color::Transparent);
			rect.setOutlineColor(sf::Color::Green);
			rect.setOutlineThickness(1.f);
			target.draw(rect, states);
		}
	}
}
