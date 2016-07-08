#ifndef TE_VECTOR_OPS_H
#define TE_VECTOR_OPS_H

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

namespace te
{
	sf::Vector2f truncate(const sf::Vector2f& v, float maxMagnitude);
	sf::Vector2f perp(const sf::Vector2f& v);

	template <typename V>
	inline auto lengthSq(const V& vector) noexcept
	{
		return vector.x * vector.x + vector.y * vector.y;
	}

	template <typename V>
	inline auto length(const V& vector) noexcept
	{
		return std::sqrt(lengthSq(vector));
	}

	template <typename V>
	auto normalize(const V& vector) noexcept
	{
		auto magnitude = length(vector);
		return magnitude == 0 ? V{ 0, 0 } : V{ vector.x / magnitude, vector.y / magnitude };
	}

	template <typename V>
	inline auto distance(const V& a, const V& b) noexcept
	{
		return length(a - b);
	}

	template <typename V>
	inline auto distanceSq(const V& a, const V& b) noexcept
	{
		return lengthSq(a - b);
	}

	inline b2Vec2 operator*(const b2Vec2& v, float speed)
	{
		return b2Vec2(v.x * speed, v.y * speed);
	}
}

#endif
