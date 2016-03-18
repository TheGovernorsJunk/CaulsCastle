#ifndef TE_STATE_H
#define TE_STATE_H

namespace sf
{
	class Time;
}

namespace te
{
	template <class EntityType>
	class State
	{
	public:
		virtual ~State() {}

		virtual void enter(EntityType& entity) {}
		virtual void execute(EntityType& entity, sf::Time dt) = 0;
		virtual void exit(EntityType& entity) {}
	};
}

#endif
