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
		MessageDispatcher(const std::shared_ptr<EntityManager>&);
		void dispatchMessage(double delay, int sender, int receiver, int msg, void* extraInfo);
		void dispatchDelayedMessages(const sf::Time& dt);

		std::shared_ptr<EntityManager> getEntityManager() const;

	private:
		void discharge(BaseGameEntity& entity, const Telegram& msg);

		std::shared_ptr<EntityManager> mpEntityManager;
		std::vector<Telegram> mPriorityQ;
	};
}

#endif
