#ifndef TE_SCENE_NODE_H
#define TE_SCENE_NODE_H

#include <SFML/Graphics.hpp>

#include <memory>
#include <vector>
#include <functional>

struct b2BodyDef;
class b2Body;
enum b2BodyType;

namespace te
{
	class Game;

	class SceneNode : public sf::Drawable
	{
	public:
		static std::unique_ptr<SceneNode> make(Game& world, const b2BodyDef*);

		virtual ~SceneNode();

		void setPosition(sf::Vector2f position);
		void setPosition(float x, float y);
		void move(sf::Vector2f ds);
		void move(float x, float y);
		sf::Vector2f getPosition() const;

		void setDrawOrder(int z);
		int getDrawOrder() const;

		sf::Transform getWorldTransform() const;

		void attachNode(std::unique_ptr<SceneNode>&& child);
		std::unique_ptr<SceneNode> detachNode(const SceneNode& child);

		void attachRigidBody(const b2BodyType&);
	protected:
		SceneNode(Game& world, const b2BodyDef*);

		b2Body& getBody();
		const b2Body& getBody() const;

	private:
		struct PendingDraw
		{
			const SceneNode* pNode;
		};

		sf::Transform getParentTransform() const;
		void draw(sf::RenderTarget&, sf::RenderStates) const;
		virtual void onDraw(sf::RenderTarget&, sf::RenderStates) const;
		void concatPendingDraws(std::vector<PendingDraw>& outQueue) const;

		Game& mWorld;
		SceneNode* mpParent;
		sf::Transformable mLocalTransformable;
		std::unique_ptr<b2Body, std::function<void(b2Body*)>> mpBody;
		std::vector<std::unique_ptr<SceneNode>> mChildren;
		int mZ;
	};

	b2BodyDef createBodyDef(sf::Vector2f position, b2BodyType type);
}

#endif
