#ifndef TE_STATE_H
#define TE_STATE_H

namespace sf
{
	class Time;
}

namespace te
{
	struct Telegram;
	template <typename EntityType>
	class StateMachine;

	template <class EntityType>
	class State
	{
	public:
		virtual ~State() {}

		virtual void enter(EntityType& entity, StateMachine<EntityType>&) {}
		virtual void execute(EntityType& entity, StateMachine<EntityType>&, const sf::Time& dt) = 0;
		virtual void exit(EntityType& entity, StateMachine<EntityType>&) {}

		virtual bool onMessage(EntityType& entity, StateMachine<EntityType>&, const Telegram& telegram) = 0;
	};
}

#endif
