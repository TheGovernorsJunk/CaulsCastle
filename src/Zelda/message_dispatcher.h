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

		struct Info { virtual ~Info() {} };
		std::unique_ptr<Info> pInfo;

		Telegram(double dt, int s, int r, int msg, std::unique_ptr<Info>&& info = nullptr)
			: dispatchTime(dt), sender(s), receiver(r), msg(msg), pInfo(std::move(info)) {}
	};

	class MessageDispatcher
	{
	public:
		static std::unique_ptr<MessageDispatcher> make(EntityManager&);

		void dispatchMessage(double delay, int sender, int receiver, int msg, std::unique_ptr<Telegram::Info>&& extraInfo = nullptr);
		void dispatchDelayedMessages(const sf::Time& dt);

	private:
		MessageDispatcher(EntityManager&);

		void discharge(BaseGameEntity& entity, const Telegram& msg);

		EntityManager& mEntityManager;
		std::vector<Telegram> mPriorityQ;
	};
}

#endif
