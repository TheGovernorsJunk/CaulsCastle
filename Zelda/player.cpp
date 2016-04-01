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

		const float speed = 48.f;
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

	void Player::update(const sf::Time& dt)
	{
		sf::Vector2f ds = mVelocity * dt.asSeconds();
		ds.x = std::signbit(ds.x) ? std::floorf(ds.x) : std::ceilf(ds.x);
		ds.y = std::signbit(ds.y) ? std::floorf(ds.y) : std::ceilf(ds.y);
		move(ds);

		BoxCollider transformedCollider = mBoxCollider.transform(getTransform());
		sf::FloatRect collision;
		while (getWorld().getMap().intersects(transformedCollider, collision))
		{
			sf::Vector2f backup;
			if (collision.width < collision.height)
			{
				backup.x = collision.left == transformedCollider.getRect().left ? collision.width : -collision.width;
			}
			else
			{
				backup.y = collision.top == transformedCollider.getRect().top ? collision.height : -collision.height;
			}
			move(backup);
			transformedCollider = mBoxCollider.transform(getTransform());
		}
	}

	bool Player::intersects(const BoxCollider& o) const
	{
		return mBoxCollider.intersects(o);
	}

	bool Player::intersects(const BoxCollider& o, sf::FloatRect& collision) const
	{
		return mBoxCollider.intersects(o, collision);
	}

	bool Player::intersects(const CompositeCollider& o) const
	{
		return mBoxCollider.intersects(o);
	}

	bool Player::intersects(const CompositeCollider& o, sf::FloatRect& collision) const
	{
		return mBoxCollider.intersects(o, collision);
	}

	void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		sf::CircleShape shape(mRadius);
		shape.setFillColor(sf::Color::Blue);
		target.draw(shape, states);
	}
}
