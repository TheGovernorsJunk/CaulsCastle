#include "player.h"
#include "message_dispatcher.h"
#include "vector_ops.h"
#include "game.h"
#include "tile_map.h"
#include "zelda_game.h"
#include "sprite_renderer.h"
#include "application.h"
#include "texture_manager.h"
#include "animator.h"

#include <cassert>

namespace te
{
	std::unique_ptr<Player> Player::make(ZeldaGame& world, const TMX::Object& playerObject)
	{
		return std::unique_ptr<Player>(new Player(world, playerObject));
	}
	Player::Player(ZeldaGame& world, const TMX::Object& playerObject)
		: BaseGameEntity(world, createBodyDef(world.getPixelToWorldTransform().transformPoint(sf::Vector2f(playerObject.x + playerObject.width / 2.f, playerObject.y + playerObject.height / 2.f)), b2_dynamicBody))
		, mRadius(1)
		, mpFixture(nullptr)
		, mpSpriteRenderer(SpriteRenderer::make(*this))
		, mpAnimator(Animator::make(world.getTextureManager(), *mpSpriteRenderer))
	{
		assert(playerObject.name == "Player");

		const sf::Transform& pixelToWorldTransform = world.getPixelToWorldTransform();

		sf::Vector2f radiusVector = pixelToWorldTransform.transformPoint({ playerObject.width / 2.f, playerObject.height / 2.f });
		mRadius = std::max(radiusVector.x, radiusVector.y);

		b2PolygonShape collider;
		sf::Vector2f boxExtents = pixelToWorldTransform.transformPoint({ playerObject.width / 2.f - 2.f, playerObject.height / 2.f - 2.f });
		collider.SetAsBox(boxExtents.x, boxExtents.y);
		mpFixture = std::unique_ptr<b2Fixture, std::function<void(b2Fixture*)>>(getBody().CreateFixture(&collider, 0), [this](b2Fixture* pFixture) {
			getBody().DestroyFixture(pFixture);
		});

		mpAnimator->setAnimation(TextureManager::getID("inigo45_en_garde"));
	}

	bool Player::handleMessage(const Telegram& msg)
	{
		bool result = false;

		b2Body& body = getBody();
		const float speed = 5.f;
		if ((msg.msg & X) > 0)
		{
			//float xAxis = *(float*)msg.extraInfo;
			//body.SetLinearVelocity(b2Vec2(xAxis * speed, body.GetLinearVelocity().y));
			//result = true;
		}
		if ((msg.msg & Y) > 0)
		{
			//float yAxis = *(float*)msg.extraInfo;
			//body.SetLinearVelocity(b2Vec2(body.GetLinearVelocity().x, yAxis * speed));
			//result = true;
			//mpAnimator->setAnimation(TextureManager::getID(yAxis > 0 ? "inigo90_en_garde" : "inigo45_en_garde"));
		}

		b2Vec2 velocity = body.GetLinearVelocity();
		if (lengthSq(velocity) > speed * speed)
		{
			body.SetLinearVelocity(normalize(velocity) * speed);
		}

		return result;
	}

	void Player::onUpdate(const sf::Time& dt)
	{
		mpAnimator->update(dt);
	}

	void Player::onDraw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getWorldTransform();
		//sf::CircleShape shape(mRadius);
		//shape.setOrigin(mRadius, mRadius);
		//shape.setFillColor(sf::Color::Blue);
		//target.draw(shape, states);

		states.transform *= getWorld().getPixelToWorldTransform();
		target.draw(*mpSpriteRenderer, states);

		// Draw collider
		//b2AABB aabb = mpFixture->GetAABB(0);
		//sf::RectangleShape rectShape(sf::Vector2f(aabb.GetExtents().x * 2, aabb.GetExtents().y * 2));
		//rectShape.setOrigin(rectShape.getSize().x / 2.f, rectShape.getSize().y / 2.f);
		//sf::Color color = sf::Color::Red;
		//color.a = 128;
		//rectShape.setFillColor(color);

		//target.draw(rectShape, states);
	}
}
