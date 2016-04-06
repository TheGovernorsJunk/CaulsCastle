#include "message_dispatcher.h"
#include "entity_manager.h"
#include "base_game_entity.h"

#include <SFML/System.hpp>

#include <algorithm>

namespace te
{
	bool operator<(const Telegram& a, const Telegram& b)
	{
		return a.dispatchTime < b.dispatchTime;
	}

	MessageDispatcher::MessageDispatcher(EntityManager& em)
		: mEntityManager(em)
		, mPriorityQ()
	{}

	void MessageDispatcher::dispatchMessage(double delay, int sender, int receiver, int msg, void* extraInfo)
	{
		Telegram telegram{ delay,sender,receiver,msg,extraInfo };
		if (delay <= 0.0)
		{
			auto pReceiver = mEntityManager.getEntityFromID(receiver);
			discharge(*pReceiver, telegram);
		}
		else
		{
			mPriorityQ.push_back(telegram);
		}
	}

	void MessageDispatcher::dispatchDelayedMessages(const sf::Time& dt)
	{
		std::for_each(mPriorityQ.begin(), mPriorityQ.end(), [this, &dt](Telegram& telegram) {
			telegram.dispatchTime -= dt.asSeconds();
			if (telegram.dispatchTime < 0)
			{
				auto pReceiver = mEntityManager.getEntityFromID(telegram.receiver);
				discharge(*pReceiver, telegram);
			}
		});
		mPriorityQ.erase(std::remove_if(mPriorityQ.begin(), mPriorityQ.end(), [](const Telegram& telegram) {
			return telegram.dispatchTime < 0;
		}), mPriorityQ.end());
	}

	EntityManager& MessageDispatcher::getEntityManager() const
	{
		return mEntityManager;
	}

	void MessageDispatcher::discharge(BaseGameEntity& entity, const Telegram& msg)
	{
		entity.handleMessage(msg);
	}
}
