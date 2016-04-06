#include "scene_node.h"
#include "game.h"

#include <Box2D/Box2D.h>

#include <algorithm>

namespace te
{
	static const float PI = 3.14159265358979323846f;

	SceneNode::~SceneNode() {}

	std::unique_ptr<SceneNode> SceneNode::make(Game& world, const b2BodyDef& bodyDef)
	{
		return std::unique_ptr<SceneNode>(new SceneNode(world, bodyDef));
	}

	SceneNode::SceneNode(Game& world, const b2BodyDef& bodyDef)
		: mWorld(world)
		, mpParent(nullptr)
		, mpBody(mWorld.getPhysicsWorld().CreateBody(&bodyDef), [this](b2Body* pBody) { mWorld.getPhysicsWorld().DestroyBody(pBody); })
		, mChildren()
		, mZ(0)
	{
		if (!mpBody) throw std::runtime_error("Unable to create b2Body in SceneNode.");
	}

	void SceneNode::setPosition(sf::Vector2f position)
	{
		position = getParentTransform() * position;
		mpBody->SetTransform(b2Vec2(position.x, position.y), mpBody->GetAngle());
	}

	void SceneNode::setPosition(float x, float y)
	{
		setPosition(sf::Vector2f(x, y));
	}

	void SceneNode::move(sf::Vector2f move)
	{
		setPosition(getPosition() + move);
	}

	void SceneNode::move(float x, float y)
	{
		move(sf::Vector2f(x, y));
	}

	sf::Vector2f SceneNode::getPosition() const
	{
		b2Vec2 worldPosition = mpBody->GetPosition();
		return getParentTransform().getInverse() * sf::Vector2f(worldPosition.x, worldPosition.y);
	}

	void SceneNode::setDrawOrder(int z)
	{
		mZ = z;
	}

	int SceneNode::getDrawOrder() const
	{
		return mZ;
	}

	sf::Transform SceneNode::getWorldTransform() const
	{
		sf::Transform transform = sf::Transform::Identity;

		b2Vec2 position = mpBody->GetPosition();
		transform.translate(sf::Vector2f(position.x, position.y));
		transform.rotate(mpBody->GetAngle() * 180.f / PI, sf::Vector2f(position.x, position.y));

		return transform;
	}

	void SceneNode::attachNode(std::unique_ptr<SceneNode>&& child)
	{
		child->mpParent = this;
		mChildren.push_back(std::move(child));
	}

	std::unique_ptr<SceneNode> SceneNode::detachNode(const SceneNode& child)
	{
		auto found = std::find_if(mChildren.begin(), mChildren.end(), [&child](auto& mChild) {
			return mChild.get() == &child;
		});

		if (found == mChildren.end()) throw std::runtime_error("Node not a child.");

		std::unique_ptr<SceneNode> result = std::move(*found);
		result->mpParent = nullptr;
		mChildren.erase(found);
		return result;
	}

	b2Body& SceneNode::getBody()
	{
		return *mpBody;
	}

	const b2Body& SceneNode::getBody() const
	{
		return *mpBody;
	}

	sf::Transform SceneNode::getParentTransform() const
	{
		return mpParent ? mpParent->getWorldTransform() : sf::Transform::Identity;
	}

	void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		std::vector<PendingDraw> pendingDraws;
		concatPendingDraws(pendingDraws);
		std::sort(pendingDraws.begin(), pendingDraws.end(), [](PendingDraw& a, PendingDraw b) {
			return a.pNode->getDrawOrder() < b.pNode->getDrawOrder();
		});
		for (auto& draw : pendingDraws)
		{
			draw.pNode->onDraw(target, states);
		}
	}

	void SceneNode::onDraw(sf::RenderTarget& target, sf::RenderStates states) const {}

	void SceneNode::concatPendingDraws(std::vector<PendingDraw>& outQueue) const
	{
		outQueue.push_back({
			this
		});
		for (auto& child : mChildren) child->concatPendingDraws(outQueue);
	}

	b2BodyDef createBodyDef(sf::Vector2f position, b2BodyType type)
	{
		b2BodyDef def;
		def.position = b2Vec2(position.x, position.y);
		def.type = type;
		return def;
	}
}
