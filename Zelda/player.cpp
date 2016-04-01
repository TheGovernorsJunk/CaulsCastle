#include "player.h"
#include "message_dispatcher.h"
#include "vector_ops.h"

#include <cassert>

namespace te
{
	Player::Player(Game& world, const TMX::Object& playerObject)
		: BaseGameEntity(world)
		, mRadius(std::max(playerObject.width / 2.f, playerObject.height / 2.f))
		, mVelocity(0, 0)
	{
		assert(playerObject.name == "Player");
		setPosition(playerObject.x + playerObject.width / 2.f, playerObject.y + playerObject.height / 2.f);
		setOrigin(playerObject.width / 2.f, playerObject.height / 2.f);
	}

	bool Player::handleMessage(const Telegram& msg)
	{
		float speed = 64.f;
		if ((msg.msg & X) > 0)
		{
			float xAxis = *(float*)msg.extraInfo;
			mVelocity.x = xAxis * speed;
			return true;
		}
		if ((msg.msg & Y) > 0)
		{
			float yAxis = *(float*)msg.extraInfo;
			mVelocity.y = yAxis * speed;
			return true;
		}
		return false;
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
		move(mVelocity * dt.asSeconds());
	}

	void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		sf::CircleShape shape(mRadius);
		shape.setFillColor(sf::Color::Blue);
		target.draw(shape, states);
	}
}
