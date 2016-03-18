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
		virtual void execute(Entity& entity, sf::Time dt)
		{
			entity.move(dt.asSeconds() * entity.getVelocity());
		}
		virtual bool onMessage(Entity& entity, const Telegram& telegram)
		{
			if (telegram.msg == 50)
			{
				std::cout << "Message received!" << std::endl;
			}
			return false;
		}
	};



	Entity::Entity(const std::shared_ptr<MessageDispatcher>& pMessageDispatcher)
		: mSprite()
		, mBoxCollider()
		, mDrawColliderEnabled(false)
		, mVelocity(0, 0)
		, mFSM(*this, std::make_shared<EntityState>())
		, mpMessageDispatcher(pMessageDispatcher)
		, mpEntityManager(pMessageDispatcher->getEntityManager())
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
