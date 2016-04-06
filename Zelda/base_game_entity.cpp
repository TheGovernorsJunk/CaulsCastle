#include "base_game_entity.h"
#include "game.h"

namespace te
{
	BaseGameEntity::BaseGameEntity(Game& world, sf::Vector2f position, b2BodyType bodyType)
		: sf::Drawable()
		, mID(UNREGISTERED_ID)
		, mBoundingRadius(1.f)
		, mWorld(world)
		, mpBody(nullptr)
	{
		b2BodyDef bodyDef;
		bodyDef.position.Set(position.x, position.y);
		bodyDef.type = bodyType;

		mpBody = std::unique_ptr<b2Body, std::function<void(b2Body*)>>(mWorld.getPhysicsWorld().CreateBody(&bodyDef), [this](b2Body* pBody) {
			mWorld.getPhysicsWorld().DestroyBody(pBody);
		});

		if (!mpBody) throw std::runtime_error("Unable to create body in BaseGameEntity ctor.");
	}

	BaseGameEntity::~BaseGameEntity() {}

	void BaseGameEntity::setPosition(sf::Vector2f position)
	{
		setPosition(position.x, position.y);
	}

	void BaseGameEntity::setPosition(float x, float y)
	{
		mpBody->SetTransform(b2Vec2(x, y), mpBody->GetAngle());
	}

	void BaseGameEntity::move(sf::Vector2f ds)
	{
		move(ds.x, ds.y);
	}

	void BaseGameEntity::move(float x, float y)
	{
		b2Vec2 pos = mpBody->GetPosition();
		mpBody->SetTransform(b2Vec2(pos.x + x, pos.y + y), mpBody->GetAngle());
	}

	//void BaseGameEntity::setOrigin(sf::Vector2f o)
	//{
	//	setOrigin(o.x, o.y);
	//}

	//void BaseGameEntity::setOrigin(float x, float y)
	//{
	//	mOrigin = { x, y };
	//	setPosition(getPosition());
	//}

	sf::Vector2f BaseGameEntity::getPosition() const
	{
		b2Vec2 pos = mpBody->GetPosition();
		return sf::Vector2f(pos.x, pos.y);
	}

	sf::Transform BaseGameEntity::getTransform() const
	{
		sf::Transform retTransform = sf::Transform::Identity;

		b2Vec2 position = mpBody->GetPosition();
		retTransform.translate(position.x, position.y);
		float angle = mpBody->GetAngle();
		retTransform.rotate(angle * 180.f / 3.14159265358979323846f);

		return retTransform;
	}

	void BaseGameEntity::setBoundingRadius(float radius)
	{
		mBoundingRadius = radius;
	}

	float BaseGameEntity::getBoundingRadius() const
	{
		return mBoundingRadius;
	}

	bool BaseGameEntity::handleMessage(const Telegram& msg)
	{
		return false;
	}

	int BaseGameEntity::getID() const
	{
		return mID;
	}

	const Game& BaseGameEntity::getWorld() const
	{
		return mWorld;
	}

	Game& BaseGameEntity::getWorld()
	{
		return mWorld;
	}

	const b2Body& BaseGameEntity::getBody() const
	{
		return *mpBody;
	}

	b2Body& BaseGameEntity::getBody()
	{
		return *mpBody;
	}

	void BaseGameEntity::draw(sf::RenderTarget&, sf::RenderStates) const {}
}
