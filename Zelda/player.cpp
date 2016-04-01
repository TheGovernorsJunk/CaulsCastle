#include "player.h"
#include "message_dispatcher.h"
#include "vector_ops.h"
#include "game.h"
#include "tile_map.h"

#include <cassert>

namespace te
{
	Player::Player(Game& world, const TMX::Object& playerObject)
		: BaseGameEntity(world)
		, mRadius(std::max(playerObject.width / 2.f, playerObject.height / 2.f))
		, mVelocity(0, 0)
		, mBoxCollider({ 0, 0, (float)playerObject.width, (float)playerObject.height })
	{
		assert(playerObject.name == "Player");
		setPosition(playerObject.x + playerObject.width / 2.f, playerObject.y + playerObject.height / 2.f);
		setOrigin(playerObject.width / 2.f, playerObject.height / 2.f);
	}

	bool Player::handleMessage(const Telegram& msg)
	{
		bool result = false;

		const float speed = 64.f;
		if ((msg.msg & X) > 0)
		{
			float xAxis = *(float*)msg.extraInfo;
			mVelocity.x = xAxis * speed;
			result = true;
		}
		if ((msg.msg & Y) > 0)
		{
			float yAxis = *(float*)msg.extraInfo;
			mVelocity.y = yAxis * speed;
			result = true;
		}

		if (lengthSq(mVelocity) > speed * speed)
		{
			mVelocity = normalize(mVelocity) * speed;
		}

		return result;
	}

	//bool Player::handleMessage(const Telegram& msg)
	//{
	//	bool result = false;

	//	float sign = ((msg.msg & On) > 0) ? 1.f : -1.f;

	//	float speed = 32.f;
	//	if ((msg.msg & Left) > 0)
	//	{
	//		mVelocity += sign * sf::Vector2f(-speed, 0);
	//		result = true;
	//	}
	//	else if ((msg.msg & Right) > 0)
	//	{
	//		mVelocity += sign * sf::Vector2f(speed, 0);
	//		result = true;
	//	}
	//	else if ((msg.msg & Up) > 0)
	//	{
	//		mVelocity += sign * sf::Vector2f(0, -speed);
	//		result = true;
	//	}
	//	else if ((msg.msg & Down) > 0)
	//	{
	//		mVelocity += sign * sf::Vector2f(0, speed);
	//		result =  true;
	//	}

	//	//if (mVelocity.x != 0 && mVelocity.y != 0)
	//	//{
	//	//	mVelocity = normalize(mVelocity) * speed;
	//	//}

	//	return result;
	//}

	void Player::update(const sf::Time& dt)
	{
		sf::Vector2f position = getPosition();
		move(mVelocity * dt.asSeconds());
		if (getWorld().getMap().intersects(mBoxCollider.transform(getTransform())))
		{
			setPosition(position);
		}
	}

	bool Player::intersects(const BoxCollider& o) const
	{
		return mBoxCollider.intersects(o);
	}

	bool Player::intersects(const CompositeCollider& o) const
	{
		return mBoxCollider.intersects(o);
	}

	void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		sf::CircleShape shape(mRadius);
		shape.setFillColor(sf::Color::Blue);
		target.draw(shape, states);
	}
}
