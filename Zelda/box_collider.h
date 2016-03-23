#ifndef TE_BOX_COLLIDER_H
#define TE_BOX_COLLIDER_H

#include "collider.h"

namespace te
{
	class BoxCollider : public Collider
	{
	public:
		BoxCollider(const sf::FloatRect& rect);

		std::vector<Wall2f> getWalls() const;

		template <typename T>
		bool contains(T x, T y) const
		{
			return mRect.contains(x, y);
		}
	private:
		virtual void draw(sf::RenderTarget&, sf::RenderStates) const;

		sf::FloatRect mRect;
	};
}

#endif
