#include "entity.h"

namespace te
{
	Entity::Entity()
		: mSprite()
		, mBoxCollider()
		, mDrawColliderEnabled(false)
		, mVelocity(0, 0)
		, mFSM(*this, nullptr)
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

	bool Entity::handleMessage(const Telegram& msg)
	{
		return false;
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
