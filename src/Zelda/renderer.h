#ifndef TE_RENDERER_H
#define TE_RENDERER_H

#include "component.h"
#include "resource_manager.h"
#include "base_game_entity.h"
#include "game.h"

#include <SFML/Graphics.hpp>

#include <memory>

namespace te
{
	class BaseGameEntity;

	template<typename T>
	class Renderer : public DrawComponent
	{
	public:
		static std::unique_ptr<Renderer<T>> make(BaseGameEntity& owner)
		{
			return std::unique_ptr<Renderer<T>>{new Renderer<T>{owner}};
		}

		void setDrawable(ResourceID<T> id)
		{
			mDrawable = mOwner.getWorld().getManager<T>().get(id);
			mResourceID = id;
		}
		//void setDrawable(T&& drawable) { mDrawable = std::move(drawable); }
		ResourceID<T> getDrawable() const { return mResourceID; }

	private:
		Renderer(BaseGameEntity& owner)
			: mOwner{owner}
			, mDrawable{}
		{}

		void draw(sf::RenderTarget& target, sf::RenderStates states) const
		{
			target.draw(mDrawable, states);
		}

		BaseGameEntity& mOwner;
		ResourceID<T> mResourceID;
		T mDrawable;
	};
}

#endif
