#include "transformable.h"

#include <Box2D/Box2D.h>

namespace te
{
	Transformable::Transformable()
		: sf::Transformable{}
	{}

	void Transformable::setPosition(float x, float y)
	{
		sf::Transformable::setPosition(x, y);
		onTransformUpdate();
	}

	void Transformable::setPosition(const sf::Vector2f& position)
	{
		sf::Transformable::setPosition(position);
		onTransformUpdate();
	}

	void Transformable::setRotation(float angle)
	{
		sf::Transformable::setRotation(angle);
		onTransformUpdate();
	}

	void Transformable::setScale(float x, float y)
	{
		sf::Transformable::setScale(x, y);
		onTransformUpdate();
	}

	void Transformable::setScale(const sf::Vector2f& factors)
	{
		sf::Transformable::setScale(factors);
		onTransformUpdate();
	}

	const sf::Vector2f& Transformable::getPosition() const
	{
		return sf::Transformable::getPosition();
	}

	float Transformable::getRotation() const
	{
		return sf::Transformable::getRotation();
	}

	const sf::Vector2f& Transformable::getScale() const
	{
		return sf::Transformable::getScale();
	}

	const sf::Vector2f& Transformable::getOrigin() const
	{
		return sf::Transformable::getOrigin();
	}

	void Transformable::move(float offsetX, float offsetY)
	{
		sf::Transformable::move(offsetX, offsetY);
		onTransformUpdate();
	}

	void Transformable::move(const sf::Vector2f& offset)
	{
		sf::Transformable::move(offset);
		onTransformUpdate();
	}

	void Transformable::rotate(float angle)
	{
		sf::Transformable::rotate(angle);
		onTransformUpdate();
	}

	void Transformable::scale(float factorX, float factorY)
	{
		sf::Transformable::scale(factorX, factorY);
		onTransformUpdate();
	}

	void Transformable::scale(const sf::Vector2f& factors)
	{
		sf::Transformable::scale(factors);
		onTransformUpdate();
	}

	const sf::Transform& Transformable::getTransform() const
	{
		return sf::Transformable::getTransform();
	}

	const sf::Transform& Transformable::getInverseTransform() const
	{
		return sf::Transformable::getInverseTransform();
	}
}
