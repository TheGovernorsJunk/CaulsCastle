#include "game.h"
#include "tile_map.h"
#include "vector_ops.h"
#include "entity_manager.h"
#include "message_dispatcher.h"
#include "scene_node.h"
#include "application.h"

#include <algorithm>
#include <iterator>

namespace te
{
	Game::Game(Application& app)
		: mApp(app)
		, mpEntityManager(EntityManager::make())
		, mpMessageDispatcher(MessageDispatcher::make(*mpEntityManager))
		, mpWorld(new b2World(b2Vec2(0, 0)))
		, mEntities()
	{}

	Game::~Game() {}

	//bool Game::isPathObstructed(sf::Vector2f a, sf::Vector2f b, float boundingRadius) const
	//{
	//	throwIfNoMap();
	//	auto& walls = mpTileMap->getWalls();

	//	sf::Vector2f toB = normalize(b - a);
	//	sf::Vector2f currPos = a;

	//	while (distanceSq(currPos, b) > boundingRadius * boundingRadius)
	//	{
	//		currPos += toB * 0.5f * boundingRadius;
	//		for (auto& wall : walls)
	//			if (wall.intersects(currPos, boundingRadius))
	//				return true;
	//	}

	//	return false;
	//}

	void Game::update(const sf::Time& dt)
	{
		mpMessageDispatcher->dispatchDelayedMessages(dt);
		mpWorld->Step(dt.asSeconds(), 8, 3);
		std::for_each(mEntities.begin(), mEntities.end(), [dt](const std::unique_ptr<BaseGameEntity>& pEntity) {
			pEntity->update(dt);
		});
		mEntities.erase(std::remove_if(mEntities.begin(), mEntities.end(), [](const std::unique_ptr<BaseGameEntity>& pEntity) {
			return pEntity->isMarkedForRemoval();
		}), mEntities.end());
	}

	//Application& Game::getApplication()
	//{
	//	return mApp;
	//}

	TextureManager& Game::getTextureManager()
	{
		return mApp.getTextureManager();
	}

	const TextureManager& Game::getTextureManager() const
	{
		return mApp.getTextureManager();
	}

	EntityManager& Game::getEntityManager() const
	{
		return *mpEntityManager;
	}

	MessageDispatcher& Game::getMessageDispatcher() const
	{
		return *mpMessageDispatcher;
	}

	b2World& Game::getPhysicsWorld() { return *mpWorld; }
	const b2World& Game::getPhysicsWorld() const { return *mpWorld; }

	//void Game::setPixelToWorldTransform(const sf::Transform& t)
	//{
	//	mPixelToWorld = t;
	//	mWorldToPixel = t.getInverse();
	//}
	//const sf::Transform& Game::getPixelToWorldTransform() const { return mPixelToWorld; }
	//const sf::Transform& Game::getWorldToPixelTransform() const { return mWorldToPixel; }

	//void Game::setTileMap(std::unique_ptr<TileMap>&& pTileMap)
	//{
	//	if (pTileMap)
	//	{
	//		mTileMapID = pTileMap->getID();
	//		mpTileMap = pTileMap.get();
	//		mpSceneGraph->attachNode(std::move(pTileMap));
	//	}
	//	else
	//	{
	//		throw std::runtime_error("Must supply TileMap to Game.");
	//	}
	//}

	void Game::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		struct PendingDraw
		{
			sf::Transform transform;
			const DrawComponent* component;
		};

		std::vector<PendingDraw> pendingDraws;
		for (auto& pEntity : mEntities)
		{
			std::vector<DrawComponent*> components{};
			pEntity->getDrawComponents(std::back_inserter(components));
			sf::Transform transform = states.transform * pEntity->getTransform();
			for (auto component : components) pendingDraws.push_back(PendingDraw{transform, component});
		}
		std::sort(pendingDraws.begin(), pendingDraws.end(), [](const PendingDraw& a, const PendingDraw& b) {
			return a.component->getDrawOrder() < b.component->getDrawOrder() || (a.component->getDrawOrder() == b.component->getDrawOrder() && a.transform.transformPoint({ 0, 0 }).y < b.transform.transformPoint({ 0, 0 }).y);
		});
		for (auto& drawable : pendingDraws) {
			states.transform = drawable.transform;
			target.draw(*drawable.component, states);
		}
	}

	void Game::addEntity(std::unique_ptr<BaseGameEntity>&& pEntity)
	{
		getEntityManager().registerEntity(*pEntity);
		mEntities.push_back(std::move(pEntity));
	}

	//void Game::throwIfNoMap() const
	//{
	//	if (!mpEntityManager->hasEntity(mTileMapID)) throw std::runtime_error("TileMap not set in Game.");
	//}
}
