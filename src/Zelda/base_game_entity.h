#ifndef TE_BASE_GAME_ENTITY_H
#define TE_BASE_GAME_ENTITY_H

#include "scene_node.h"
#include "typedefs.h"
#include "component.h"

#include <SFML/Graphics.hpp>

#include <memory>
#include <functional>
#include <vector>

namespace te
{
	struct Telegram;
	class EntityManager;
	class Game;
	class Shape;

	class BaseGameEntity : private sf::Transformable, public sf::Drawable
	{
	public:
		const static int UNREGISTERED_ID = 0;

		BaseGameEntity(Game& pWorld, sf::Vector2f position);
		BaseGameEntity(Game& World);
		virtual ~BaseGameEntity();

		void update(const sf::Time& dt);
		void setBoundingRadius(float radius);
		float getBoundingRadius() const;
		virtual bool handleMessage(const Telegram& msg);
		EntityID getID() const;
		const Game& getWorld() const;
		Game& getWorld();
		void setDrawOrder(int z);
		int getDrawOrder() const;
		const sf::Transform& getTransform() const;
		void setPosition(const sf::Vector2f& position) { sf::Transformable::setPosition(position); }
		const sf::Vector2f& getPosition() const;
		void move(float x, float y) { sf::Transformable::move(x, y); }
		void die() { mMarkedForRemoval = true; }
		bool isMarkedForRemoval() const { return mMarkedForRemoval; }

		template<typename Component, typename... Args>
		Component& addComponent(Args&&... args)
		{
			auto upComponent = Component::make(*this, std::forward<Args>(args)...);
			Component* pComponent = upComponent.get();
			mComponents.push_back(std::move(upComponent));
			return *pComponent;
		}

		template<typename Component>
		bool hasComponent() const
		{
			return getComponent<Component>() != nullptr;
		}

		template<typename Component>
		Component* getComponent() const
		{
			Component* pComponent = nullptr;
			for (auto& component : mComponents)
			{
				pComponent = dynamic_cast<Component*>(component.get());
				if (pComponent) break;
			}
			return pComponent;
		}

	protected:
		virtual void onUpdate(const sf::Time& dt) {}

	private:
		friend class EntityManager;

		void draw(sf::RenderTarget&, sf::RenderStates) const {}

		int mID;
		int mZ;
		bool mMarkedForRemoval;
		float mBoundingRadius;
		std::vector<std::unique_ptr<Component>> mComponents;
		Game& mWorld;
	};
}

#endif
