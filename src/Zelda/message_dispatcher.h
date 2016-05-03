#ifndef TE_MESSAGE_DISPATCHER_H
#define TE_MESSAGE_DISPATCHER_H

#include <vector>
#include <memory>

namespace sf
{
	class Time;
}

namespace te
{
	class BaseGameEntity;
	class EntityManager;

	struct Telegram
	{
		double dispatchTime;
		int sender;
		int receiver;
		int msg;
		void* extraInfo;
	};

	bool operator<(const Telegram& a, const Telegram& b);

	class MessageDispatcher
	{
	public:
		static std::unique_ptr<MessageDispatcher> make(EntityManager&);

		void dispatchMessage(double delay, int sender, int receiver, int msg, void* extraInfo = NULL);
		void dispatchDelayedMessages(const sf::Time& dt);

	private:
		MessageDispatcher(EntityManager&);

		void discharge(BaseGameEntity& entity, const Telegram& msg);

		EntityManager& mEntityManager;
		std::vector<Telegram> mPriorityQ;
	};
}

#endif
