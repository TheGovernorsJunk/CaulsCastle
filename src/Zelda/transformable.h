#ifndef TE_TRANSFORMABLE_H
#define TE_TRANSFORMABLE_H

#include <SFML/Graphics/Transformable.hpp>

namespace te
{
	class Transformable : private sf::Transformable
	{
	public:
		Transformable();
		virtual ~Transformable() {}

		void setPosition(float x, float y);
		void setPosition(const sf::Vector2f& position);

		void setRotation(float angle);

		void setScale(float factorX, float factorY);
		void setScale(const sf::Vector2f& factors);

		const sf::Vector2f& getPosition() const;

		float getRotation() const;

		const sf::Vector2f& getScale() const;

		const sf::Vector2f& getOrigin() const;

		void move(float offsetX, float offsetY);
		void move(const sf::Vector2f& offset);

		void rotate(float angle);

		void scale(float factorX, float factorY);
		void scale(const sf::Vector2f& factors);

		const sf::Transform& getTransform() const;
		const sf::Transform& getInverseTransform() const;

	protected:
		virtual void onTransformUpdate() const {}
	};
}

#endif
