#ifndef TE_BASE_GAME_ENTITY_H
#define TE_BASE_GAME_ENTITY_H

#include <SFML/Graphics.hpp>

#include <memory>
#include <functional>

#include <Box2D/Box2D.h>

namespace sf
{
	class Time;
}

namespace te
{
	struct Telegram;
	class EntityManager;
	class Game;

	class BaseGameEntity : public sf::Drawable
	{
	public:
		const static int UNREGISTERED_ID = 0;

		BaseGameEntity(Game& pWorld, sf::Vector2f position = { 0, 0 }, b2BodyType = b2_dynamicBody);
		virtual ~BaseGameEntity();

		void setPosition(sf::Vector2f pos);
		void setPosition(float x, float y);
		void move(sf::Vector2f ds);
		void move(float x, float y);
		void setOrigin(sf::Vector2f o);
		void setOrigin(float x, float y);
		sf::Vector2f getPosition() const;
		sf::Transform getTransform() const;

		void setBoundingRadius(float radius);
		float getBoundingRadius() const;
		virtual void update(const sf::Time& dt) = 0;
		virtual bool handleMessage(const Telegram& msg);
		int getID() const;
		const Game& getWorld() const;
		Game& getWorld();
	protected:
		const b2Body& getBody() const;
		b2Body& getBody();

	private:
		virtual void draw(sf::RenderTarget&, sf::RenderStates) const;
		friend class EntityManager;

		int mID;
		float mBoundingRadius;
		Game& mWorld;
		std::unique_ptr<b2Body, std::function<void(b2Body*)>> mpBody;
		b2Vec2 mOrigin;
	};
}

#endif
