#include "scene_node.h"
#include "game.h"
#include "utilities.h"

#include <Box2D/Box2D.h>

#include <algorithm>
#include <iterator>

namespace te
{
	SceneNode::~SceneNode() {}

	std::unique_ptr<SceneNode> SceneNode::make(Game& world, const b2BodyDef& bodyDef)
	{
		return std::unique_ptr<SceneNode>(new SceneNode(world, bodyDef));
	}

	std::unique_ptr<SceneNode> SceneNode::make(Game& world, sf::Vector2f position)
	{
		return std::unique_ptr<SceneNode>(new SceneNode(world, position));
	}

	SceneNode::SceneNode(Game& world, const b2BodyDef& bodyDef)
		: mWorld(world)
		, mpParent(nullptr)
		, mLocalTransformable()
		, mpBody(mWorld.getPhysicsWorld().CreateBody(&bodyDef), [this](b2Body* pBody) { mWorld.getPhysicsWorld().DestroyBody(pBody); })
		, mChildren()
		, mZ(0)
	{
		if (!mpBody) throw std::runtime_error("Unable to create b2Body in SceneNode.");
		b2Vec2 pos = mpBody->GetPosition();
		mLocalTransformable.setPosition(pos.x, pos.y);
	}

	SceneNode::SceneNode(Game& world, sf::Vector2f position)
		: mWorld(world)
		, mpParent(nullptr)
		, mLocalTransformable()
		, mpBody(nullptr)
		, mChildren()
		, mZ(0)
	{
		mLocalTransformable.setPosition(position);
	}

	void SceneNode::setPosition(sf::Vector2f position)
	{
		if (mpBody)
		{
			sf::Vector2f worldPosition = getParentTransform() * position;
			mpBody->SetTransform(b2Vec2(worldPosition.x, worldPosition.y), mpBody->GetAngle());
		}
		mLocalTransformable.setPosition(position);
		transformChildren();
	}

	void SceneNode::setPosition(float x, float y)
	{
		setPosition(sf::Vector2f(x, y));
	}

	void SceneNode::move(sf::Vector2f offset)
	{
		setPosition(getPosition() + offset);
	}

	void SceneNode::move(float x, float y)
	{
		move(sf::Vector2f(x, y));
	}

	sf::Vector2f SceneNode::getPosition() const
	{
		if (mpBody)
		{
			b2Vec2 worldPosition = mpBody->GetPosition();
			return getParentTransform().getInverse() * sf::Vector2f(worldPosition.x, worldPosition.y);
		}
		else
		{
			return mLocalTransformable.getPosition();
		}
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
		if (mpBody)
		{
			sf::Transform transform = sf::Transform::Identity;

			b2Vec2 position = mpBody->GetPosition();
			transform.translate(sf::Vector2f(position.x, position.y));
			transform.rotate(mpBody->GetAngle() * 180.f / PI, sf::Vector2f(position.x, position.y));

			return transform;
		}
		else
		{
			return getParentTransform() * mLocalTransformable.getTransform();
		}
	}

	void SceneNode::attachNode(std::unique_ptr<SceneNode>&& child)
	{
		child->mpParent = this;
		child->refresh();
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
		result->refresh();
		mChildren.erase(found);
		return result;
	}

	SceneNode& SceneNode::getParent() const
	{
		if (mpParent) return *mpParent;
		throw std::runtime_error{"No parent on node."};
	}

	void SceneNode::attachRigidBody(const b2BodyType& bodyType)
	{
		if (!mpBody)
		{
			b2BodyDef bodyDef;
			bodyDef.type = bodyType;
			sf::Vector2f worldPosition = getWorldTransform().transformPoint({ 0, 0 });
			bodyDef.position = { worldPosition.x, worldPosition.y };
			mpBody = { mWorld.getPhysicsWorld().CreateBody(&bodyDef), [this](b2Body* pBody) { mWorld.getPhysicsWorld().DestroyBody(pBody); } };
		}
		else
		{
			throw std::runtime_error("Node already has rigid body.");
		}
	}

	void SceneNode::update(const sf::Time& dt)
	{
		onUpdate(dt);
		for (auto& child : mChildren) child->update(dt);
	}

	b2Body& SceneNode::getBody()
	{
		if (mpBody)
		{
			return *mpBody;
		}
		throw std::runtime_error("Rigid body not set.");
	}

	const b2Body& SceneNode::getBody() const
	{
		if (mpBody)
		{
			return *mpBody;
		}
		throw std::runtime_error("Rigid body not set.");
	}

	void SceneNode::refresh()
	{
		// Triggers any needed Box2D updates
		setPosition(mLocalTransformable.getPosition());
	}

	void SceneNode::transformChildren()
	{
		for (auto& child : mChildren)
			child->refresh();
	}

	sf::Transform SceneNode::getParentTransform() const
	{
		return mpParent ? mpParent->getWorldTransform() : sf::Transform::Identity;
	}

	void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		std::vector<const SceneNode*> pendingDraws;
		concatPendingDraws(std::back_inserter(pendingDraws));
		std::sort(pendingDraws.begin(), pendingDraws.end(), [](const SceneNode* a, const SceneNode* b) {
			return a->getDrawOrder() < b->getDrawOrder();
		});
		for (auto& draw : pendingDraws) draw->onDraw(target, states);
	}

	void SceneNode::onDraw(sf::RenderTarget& target, sf::RenderStates states) const {}

	template<typename Iter>
	void SceneNode::concatPendingDraws(Iter out) const
	{
		*out++ = this;
		for (auto& child : mChildren) child->concatPendingDraws(out);
	}

	void SceneNode::onUpdate(const sf::Time& dt) {}

	b2BodyDef createBodyDef(sf::Vector2f position, b2BodyType type)
	{
		b2BodyDef def;
		def.position = b2Vec2(position.x, position.y);
		def.type = type;
		return def;
	}
}
